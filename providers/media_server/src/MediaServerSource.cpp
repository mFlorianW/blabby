// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaServerSource.hpp"
#include "BrowseResponse.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>

namespace Provider
{

MediaServerSource::MediaServerSource(std::unique_ptr<UPnPAV::MediaServer> mediaServer)
    : Multimedia::Source{mediaServer->name(),
                         mediaServer->iconUrl().isEmpty()
                             ? QStringLiteral("qrc:/mediaserverprovider/icons/24x24/PC.svg")
                             : mediaServer->iconUrl().toString()}
    , mServer{std::move(mediaServer)}
{
    navigate(QStringLiteral("0"));
}

MediaServerSource::~MediaServerSource() = default;

void MediaServerSource::navigateTo(QString const &path) noexcept
{
    navigate(path);
}

void MediaServerSource::navigate(QString const &path) noexcept
{
    mBrowseRequest = {
        .mRequest = mServer->browse(path, UPnPAV::MediaServer::BrowseFlag::DirectChildren, QString(""), QString("")),
        .mPath = path,
    };
    connect(mBrowseRequest.mRequest.get(),
            &UPnPAV::PendingSoapCall::finished,
            this,
            &MediaServerSource::onBrowseRequestFinished);
}

void MediaServerSource::onBrowseRequestFinished() noexcept
{
    if (mBrowseRequest.mRequest->hasError()) {
        qCritical(mediaServerSource) << "Browse reqeust failed with error: Error Code:"
                                     << mBrowseRequest.mRequest->errorCode()
                                     << "Error Message:" << mBrowseRequest.mRequest->errorDescription();
    }

    const auto result = mBrowseRequest.mRequest->resultAs<UPnPAV::BrowseResponse>();
    mMediaItems.clear();
    for (auto const &obj : result->objects()) {
        const auto type = obj.typeClass().contains("storageFolder") ? Multimedia::ItemType::Container
                                                                    : Multimedia::ItemType::Playable;
        mMediaItems.emplace_back(Multimedia::ItemBuilder{}
                                     .withItemType(type)
                                     .withMainText(obj.title())
                                     .withPath(obj.id())
                                     .withPlayUrl(obj.playUrl())
                                     .withSupportedTypes(obj.supportedProtocols())
                                     .build());
    }
    Q_EMIT navigationFinished(mBrowseRequest.mPath);
}

} // namespace Provider
