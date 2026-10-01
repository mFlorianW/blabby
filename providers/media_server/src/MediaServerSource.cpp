// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerSource.hpp"
#include "BrowseResponse.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <algorithm>

namespace Provider::MediaServer
{

namespace
{
/**
 * The optional properties a browse asks for, without a filter servers may omit them.
 * The resources carry the play URL, the album art, the artist and the creator are shown on the tiles, the album and
 * the duration in the Queue.
 */
constexpr auto BrowseFilter = QLatin1StringView{"res,res@duration,upnp:albumArtURI,upnp:artist,dc:creator,upnp:album"};

/**
 * How many Items a Browse asks for at once, a MediaServer may cap a page to fewer Items.
 */
constexpr auto PageSize = quint32{100};

/**
 * Gives the secondary text of an object: its artist or, without an artist, its creator.
 */
QString secondaryText(UPnPAV::MediaServerObject const& obj)
{
    return obj.artist().isEmpty() ? obj.creator() : obj.artist();
}

/**
 * Gives the duration of the resource that plays the object, unset when unknown.
 */
std::optional<std::chrono::milliseconds> durationOf(UPnPAV::MediaServerObject const& obj)
{
    auto const& resources = obj.resources();
    auto const resource = std::ranges::find(resources, obj.playUrl(), &UPnPAV::Resource::uri);
    return resource != resources.cend() ? resource->duration : std::nullopt;
}

/**
 * Gives the Items of the objects that a Browse returned.
 */
Multimedia::Items itemsOf(UPnPAV::BrowseResponse const& response)
{
    auto items = Multimedia::Items{};
    for (auto const& obj : response.objects()) {
        // Every UPnP container class (folders, albums, artists, genres, playlists, ...) derives from object.container.
        auto const type = obj.typeClass().startsWith(QStringLiteral("object.container"))
                              ? Multimedia::ItemType::Container
                              : Multimedia::ItemType::Playable;
        items.emplace_back(Multimedia::ItemBuilder{}
                               .withItemType(type)
                               .withMainText(obj.title())
                               .withSecondaryText(secondaryText(obj))
                               .withArtworkUrl(obj.albumArtUrl())
                               .withPath(obj.id())
                               .withPlayUrl(obj.playUrl())
                               .withAlbum(obj.album())
                               .withDuration(durationOf(obj))
                               .withSupportedTypes(obj.supportedProtocols())
                               .build());
    }
    return items;
}
} // namespace

Source::Source(std::unique_ptr<UPnPAV::MediaServer> mediaServer)
    : Multimedia::Source{mediaServer->name(),
                         mediaServer->iconUrl().isEmpty()
                             ? QStringLiteral("qrc:/mediaserverprovider/icons/24x24/PC.svg")
                             : mediaServer->iconUrl().toString()}
    , mServer{std::move(mediaServer)}
{
    navigate(QStringLiteral("0"), 0);
}

Source::~Source() = default;

void Source::navigate(QString const& path, qsizetype minimumItemCount) noexcept
{
    mNavigation = {.mPath = path, .mMinimumItemCount = minimumItemCount, .mItems = {}};
    browse(path, BrowseKind::Navigation, 0, minimumItemCount);
}

void Source::loadMore() noexcept
{
    if (mBrowseRequest.mPending or not canLoadMore()) {
        return;
    }
    // The next page starts after the loaded Items, a capped MediaServer may have returned fewer than requested.
    browse(mCurrentPath, BrowseKind::NextPage, mMediaItems.size(), PageSize);
}

void Source::browse(QString const& path, BrowseKind kind, qsizetype startingIndex, qsizetype requestedCount) noexcept
{
    mBrowseRequest = {
        .mRequest = mServer->browse(path,
                                    UPnPAV::MediaServer::BrowseFlag::DirectChildren,
                                    BrowseFilter,
                                    QString(""),
                                    static_cast<quint32>(startingIndex),
                                    static_cast<quint32>(std::max(requestedCount, qsizetype{PageSize}))),
        .mKind = kind,
        .mPending = true,
    };
    connect(mBrowseRequest.mRequest.get(), &UPnPAV::PendingSoapCall::finished, this, &Source::onBrowseRequestFinished);
}

void Source::onBrowseRequestFinished() noexcept
{
    mBrowseRequest.mPending = false;
    auto const isNavigation = mBrowseRequest.mKind == BrowseKind::Navigation;
    if (mBrowseRequest.mRequest->hasError()) {
        qCritical(mediaServerSource) << "Browse reqeust failed with error: Error Code:"
                                     << mBrowseRequest.mRequest->errorCode()
                                     << "Error Message:" << mBrowseRequest.mRequest->errorDescription();
        if (isNavigation) {
            mNavigation.mItems.clear();
            Q_EMIT navigationFailed(mNavigation.mPath);
        } else {
            Q_EMIT loadingMoreFailed();
        }
        return;
    }

    auto const result = mBrowseRequest.mRequest->resultAs<UPnPAV::BrowseResponse>();
    auto const totalMatches = qsizetype{result->totalMatches()};
    if (isNavigation) {
        finishNavigationPage(itemsOf(*result), totalMatches);
    } else {
        finishNextPage(itemsOf(*result), totalMatches);
    }
}

void Source::finishNavigationPage(Multimedia::Items page, qsizetype totalMatches) noexcept
{
    auto const pageIsEmpty = page.isEmpty();
    mNavigation.mItems.append(std::move(page));
    auto const loadedCount = mNavigation.mItems.size();
    if (not pageIsEmpty and loadedCount < mNavigation.mMinimumItemCount and loadedCount < totalMatches) {
        browse(mNavigation.mPath, BrowseKind::Navigation, loadedCount, mNavigation.mMinimumItemCount - loadedCount);
        return;
    }

    mMediaItems = std::exchange(mNavigation.mItems, {});
    // A page without Items ends loading.
    setTotalItemCount(pageIsEmpty ? loadedCount : totalMatches);
    mCurrentPath = mNavigation.mPath;
    Q_EMIT navigationFinished(mCurrentPath);
}

void Source::finishNextPage(Multimedia::Items page, qsizetype totalMatches) noexcept
{
    // The latest total wins when the Container changed between pages, a page without Items ends loading.
    setTotalItemCount(page.isEmpty() ? mMediaItems.size() : totalMatches);
    mMediaItems.append(std::move(page));
    Q_EMIT moreItemsLoaded();
}

} // namespace Provider::MediaServer
