// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerView.h"
#include "IMediaServer.h"
#include "IServiceProvider.h"
#include "InvalidDeviceDescription.h"
#include "MediaServerModel.h"
#include <QDebug>

namespace MediaServer::Plugin
{

MediaServerView::MediaServerView(MediaServerModel *model, UPnPAV::IMediaServerFactory *mediaServerFab,
                                 UPnPAV::IServiceProviderFactory *serviceProviderFab)
    : QObject()
    , mServiceProviderFactory{ serviceProviderFab }
    , mMediaServerFactory{ mediaServerFab }
    , mMediaServerModel{ model }
{
    mServiceProvider = mServiceProviderFactory->createServiceProvider("urn:schemas-upnp-org:device:MediaServer:1");
    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::serviceConnected, this, &MediaServerView::onServiceConnected);
    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::serviceDisconnected, this, &MediaServerView::onServerDisconnected);
}

MediaServerView::~MediaServerView() = default;

MediaServerModel *MediaServerView::mediaServerModel() const noexcept
{
    return mMediaServerModel;
}

void MediaServerView::searchMediaServer() const noexcept
{
    if(mServiceProvider == nullptr)
    {
        return;
    }

    mServiceProvider->startSearch();
}

void MediaServerView::setActiveMediaServer(qint32 index) noexcept
{
    mActiveIndex = index;
    Q_EMIT activeMediaServerChanged();
}

UPnPAV::IMediaServer *MediaServerView::activeMediaServer() const noexcept
{
    auto mediaServerIter = std::next(mMediaServers.begin(), mActiveIndex);
    return mediaServerIter->second.get();
}

void MediaServerView::onServiceConnected(const QString &usn)
{
    if((mMediaServerModel == nullptr) || (mServiceProvider == nullptr) || (mMediaServerFactory == nullptr) ||
       (mServiceProviderFactory == nullptr) || (mMediaServers.find(usn) != mMediaServers.end()))
    {
        return;
    }

    try
    {
        auto mediaServer = mMediaServerFactory->createMediaServer(mServiceProvider->rootDeviceDescription(usn));
        mMediaServerModel->insert(mediaServer.get());
        mMediaServers[usn] = std::move(mediaServer);
    }
    catch(const UPnPAV::InvalidDeviceDescription &e)
    {
        qCritical() << e.what();
    }
}

void MediaServerView::onServerDisconnected(const QString &usn)
{
    if((mMediaServerModel == nullptr) || (mServiceProvider == nullptr) || (mMediaServerFactory == nullptr) ||
       (mServiceProviderFactory == nullptr))
    {
        return;
    }

    try
    {
        mMediaServerModel->removeServer(mMediaServers.at(usn).get());
        mMediaServers.erase(usn);
    }
    catch(const std::out_of_range &e)
    {
        qCritical() << e.what();
    }
}

} // namespace MediaServer::Plugin
