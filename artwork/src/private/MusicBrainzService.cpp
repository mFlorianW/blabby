// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MusicBrainzService.hpp"
#include "LoggingCategories.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <algorithm>

using namespace std::chrono_literals;

namespace Artwork
{

namespace
{

constexpr auto minimumSearchInterval = 1s;
constexpr auto minimumScore = 90;
// A search by the album title alone matches more unrelated albums, so it must be more confident.
constexpr auto minimumScoreWithoutArtist = 95;

QNetworkRequest createRequest(QUrl const& url)
{
    auto request = QNetworkRequest{url};
    request.setRawHeader("User-Agent",
                         QByteArrayLiteral("Blabby/" BLABBY_VERSION " ( https://github.com/mFlorianW/blabby )"));
    return request;
}

/**
 * Gives a term of a Lucene query that matches the value as a phrase.
 */
QString phrase(QString value)
{
    value.replace(QLatin1Char('\\'), QStringLiteral(R"(\\)")).replace(QLatin1Char('"'), QStringLiteral(R"(\")"));
    return QLatin1Char('"') + value + QLatin1Char('"');
}

QUrl searchUrl(QString const& artist, QString const& album)
{
    auto query = QStringLiteral("releasegroup:") + phrase(album);
    if (not artist.isEmpty()) {
        query += QStringLiteral(" AND artist:") + phrase(artist);
    }

    // The query is percent encoded on its own, so that e.g. a "+" or a "&" in a name stays part of the name.
    auto url = QUrl{QStringLiteral("https://musicbrainz.org/ws/2/release-group/")};
    url.setQuery(QStringLiteral("query=%1&fmt=json&limit=1").arg(QString::fromLatin1(QUrl::toPercentEncoding(query))),
                 QUrl::StrictMode);
    return url;
}

/**
 * Gives the id of the best matching release group of a MusicBrainz search, empty without a confident match.
 */
QString bestMatch(QByteArray const& searchResult, int minimumScore)
{
    auto const releaseGroups = QJsonDocument::fromJson(searchResult).object().value(QStringLiteral("release-groups"));
    auto const best = releaseGroups.toArray().at(0).toObject();
    auto const score = best.value(QStringLiteral("score"));
    auto const scoreValue = score.isString() ? score.toString().toInt() : score.toInt();
    if (scoreValue < minimumScore) {
        return {};
    }
    return best.value(QStringLiteral("id")).toString();
}

} // namespace

MusicBrainzService::MusicBrainzService(HttpBackend& backend, Scheduler& scheduler)
    : mBackend{backend}
    , mScheduler{scheduler}
{
    (void)connect(&mScheduler, &Scheduler::wokeUp, this, &MusicBrainzService::searchNext);
}

MusicBrainzService::~MusicBrainzService() = default;

std::shared_ptr<Request> MusicBrainzService::lookUp(QString const& artist, QString const& album)
{
    auto request = std::make_shared<Request>();
    (void)connect(request.get(), &Request::cancelled, this, [this, request = request.get()] {
        onCancelled(request);
    });
    mPendingSearches.push_back(PendingSearch{.artist = artist, .album = album, .request = request});
    searchNext();
    return request;
}

void MusicBrainzService::searchNext()
{
    if (mSearchReply != nullptr or mPendingSearches.empty()) {
        return;
    }

    auto const now = mScheduler.now();
    if (mLastSearchAt.has_value() and now < mLastSearchAt.value() + minimumSearchInterval) {
        mScheduler.wakeUpAt(mLastSearchAt.value() + minimumSearchInterval);
        return;
    }

    auto search = std::move(mPendingSearches.back());
    mPendingSearches.pop_back();
    qCDebug(artwork) << "Searching on MusicBrainz for the album" << search.album << "of" << search.artist;

    auto request = createRequest(searchUrl(search.artist, search.album));
    request.setRawHeader("Accept", "application/json");
    mLastSearchAt = now;
    mSearchRequest = std::move(search.request);
    mSearchMinimumScore = search.artist.isEmpty() ? minimumScoreWithoutArtist : minimumScore;
    mSearchReply = mBackend.get(request);
    (void)connect(mSearchReply.get(), &HttpReply::finished, this, &MusicBrainzService::onSearchFinished);
}

void MusicBrainzService::onSearchFinished()
{
    // The reply is the sender, it's deleted once its signal is handled.
    auto* const reply = mSearchReply.release();
    reply->deleteLater();
    auto request = std::move(mSearchRequest);

    auto const releaseGroupId = reply->status() == 200 ? bestMatch(reply->body(), mSearchMinimumScore) : QString{};
    if (releaseGroupId.isEmpty()) {
        qCDebug(artwork) << "MusicBrainz has no confident match, status" << reply->status();
        request->finish(QImage{});
    } else {
        fetchCover(releaseGroupId, std::move(request));
    }

    searchNext();
}

void MusicBrainzService::fetchCover(QString const& releaseGroupId, std::shared_ptr<Request> request)
{
    auto const url = QUrl{QStringLiteral("https://coverartarchive.org/release-group/%1/front-500").arg(releaseGroupId)};
    auto& fetch = mCoverFetches.emplace_back(
        CoverFetch{.reply = mBackend.get(createRequest(url)), .request = std::move(request)});
    (void)connect(fetch.reply.get(), &HttpReply::finished, this, [this, reply = fetch.reply.get()] {
        onCoverFetched(reply);
    });
}

void MusicBrainzService::onCoverFetched(HttpReply const* reply)
{
    auto const fetch = std::ranges::find_if(mCoverFetches, [reply](auto const& fetch) {
        return fetch.reply.get() == reply;
    });
    if (fetch == mCoverFetches.end()) {
        return;
    }

    // The reply is the sender, it's deleted once its signal is handled.
    fetch->reply.release()->deleteLater();
    auto const request = std::move(fetch->request);
    mCoverFetches.erase(fetch);

    auto const image = reply->status() == 200 ? QImage::fromData(reply->body()) : QImage{};
    qCDebug(artwork) << "The Cover Art Archive gave" << (image.isNull() ? "no front cover" : "the front cover")
                     << "status" << reply->status();
    request->finish(image);
}

void MusicBrainzService::onCancelled(Request const* request)
{
    std::erase_if(mPendingSearches, [request](auto const& search) {
        return search.request.get() == request;
    });
    mCoverFetches.remove_if([request](auto const& fetch) {
        return fetch.request.get() == request;
    });
    if (mSearchRequest.get() == request) {
        mSearchReply.reset();
        mSearchRequest.reset();
        searchNext();
    }
}

} // namespace Artwork
