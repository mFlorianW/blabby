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
} // namespace

Source::Source(std::unique_ptr<UPnPAV::MediaServer> mediaServer)
    : Multimedia::Source{mediaServer->name(),
                         mediaServer->iconUrl().isEmpty()
                             ? QStringLiteral("qrc:/mediaserverprovider/icons/24x24/PC.svg")
                             : mediaServer->iconUrl().toString()}
    , mServer{std::move(mediaServer)}
{
    browse(QStringLiteral("0"), BrowseKind::Navigation);
}

Source::~Source() = default;

void Source::navigateTo(QString const& path) noexcept
{
    browse(path, BrowseKind::Navigation);
}

void Source::loadMore() noexcept
{
    if (mBrowseRequest.mPending or not canLoadMore()) {
        return;
    }
    browse(mCurrentPath, BrowseKind::NextPage);
}

void Source::browse(QString const& path, BrowseKind kind) noexcept
{
    // The next page starts after the loaded Items, a capped MediaServer may have returned fewer than requested.
    auto const startingIndex = kind == BrowseKind::NextPage ? static_cast<quint32>(mMediaItems.size()) : quint32{0};
    mBrowseRequest = {
        .mRequest = mServer->browse(path,
                                    UPnPAV::MediaServer::BrowseFlag::DirectChildren,
                                    BrowseFilter,
                                    QString(""),
                                    startingIndex,
                                    PageSize),
        .mPath = path,
        .mKind = kind,
        .mPending = true,
    };
    connect(mBrowseRequest.mRequest.get(), &UPnPAV::PendingSoapCall::finished, this, &Source::onBrowseRequestFinished);
}

void Source::onBrowseRequestFinished() noexcept
{
    mBrowseRequest.mPending = false;
    auto const isNextPage = mBrowseRequest.mKind == BrowseKind::NextPage;
    if (mBrowseRequest.mRequest->hasError()) {
        qCritical(mediaServerSource) << "Browse reqeust failed with error: Error Code:"
                                     << mBrowseRequest.mRequest->errorCode()
                                     << "Error Message:" << mBrowseRequest.mRequest->errorDescription();
        if (isNextPage) {
            Q_EMIT loadingMoreFailed();
        } else {
            Q_EMIT navigationFailed(mBrowseRequest.mPath);
        }
        return;
    }

    auto const result = mBrowseRequest.mRequest->resultAs<UPnPAV::BrowseResponse>();
    if (not isNextPage) {
        mMediaItems.clear();
    }
    for (auto const& obj : result->objects()) {
        // Every UPnP container class (folders, albums, artists, genres, playlists, ...) derives from object.container.
        auto const type = obj.typeClass().startsWith(QStringLiteral("object.container"))
                              ? Multimedia::ItemType::Container
                              : Multimedia::ItemType::Playable;
        mMediaItems.emplace_back(Multimedia::ItemBuilder{}
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
    // The latest total wins when the Container changed between pages, a page without Items ends loading.
    setTotalItemCount(result->objects().isEmpty() ? mMediaItems.size() : qsizetype{result->totalMatches()});

    if (isNextPage) {
        Q_EMIT moreItemsLoaded();
    } else {
        mCurrentPath = mBrowseRequest.mPath;
        Q_EMIT navigationFinished(mBrowseRequest.mPath);
    }
}

} // namespace Provider::MediaServer
