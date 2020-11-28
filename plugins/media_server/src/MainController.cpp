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
#include "MainController.h"
#include "IMediaServer.h"
#include "IServiceProvider.h"
#include "InvalidDeviceDescription.h"
#include "MediaServerModel.h"
#include <QDebug>

namespace MediaServerPlugin
{

MainController::MainController()
    : QQuickItem()
{
}

MainController::~MainController() = default;

UPnPAV::IServiceProviderFactory *MainController::serviceProviderFactory() const noexcept
{
    return mServiceProviderFactory;
}

void MainController::setServiceProviderFactory(UPnPAV::IServiceProviderFactory *serviceProviderFactory) noexcept
{
    if((mServiceProviderFactory == serviceProviderFactory) || (serviceProviderFactory == nullptr))
    {
        return;
    }

    mServiceProviderFactory = serviceProviderFactory;
    mServiceProvider = mServiceProviderFactory->createServiceProvider("urn:schemas-upnp-org:device:MediaServer:1");
    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::serviceConnected, this, &MainController::onServiceConnected);
    Q_EMIT serviceProviderChanged();
}

UPnPAV::IMediaServerFactory *MainController::mediaServerFactory() const noexcept
{
    return mMediaServerFactory;
}
void MainController::setMediaServerFactory(UPnPAV::IMediaServerFactory *mediaServerFactory) noexcept
{
    if(mMediaServerFactory == mediaServerFactory)
    {
        return;
    }

    mMediaServerFactory = mediaServerFactory;
    Q_EMIT mediaServerFactoryChanged();
}

MediaServerModel *MainController::mediaServerModel() const noexcept
{
    return mMediaServerModel;
}

void MainController::setMediaServerModel(MediaServerModel *mediaServerModel) noexcept
{
    if(mMediaServerModel == mediaServerModel)
    {
        return;
    }

    mMediaServerModel = mediaServerModel;
    Q_EMIT mediaServerModelChanged();
}

void MainController::searchMediaServer() const noexcept
{
    if(mServiceProvider == nullptr)
    {
        return;
    }

    mServiceProvider->startSearch();
}

void MainController::onServiceConnected(const QString &usn)
{
    if(mMediaServerModel == nullptr || mServiceProvider == nullptr || mMediaServerFactory == nullptr || mServiceProviderFactory == nullptr)
    {
        return;
    }

    try
    {
        auto mediaServer = mMediaServerFactory->createMediaServer(mServiceProvider->rootDeviceDescription(usn));
        mMediaServerModel->insert(mediaServer.get());
        mMediaServers.insert(std::make_pair(usn, std::move(mediaServer)));
    }
    catch(const UPnPAV::InvalidDeviceDescription &e)
    {
        qCritical() << e.what();
    }
}

} // namespace MediaServerPlugin
