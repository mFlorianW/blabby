/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
