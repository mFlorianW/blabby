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
#include "MainControllerShould.h"
#include "MainController.h"
#include "MediaServerDouble.h"
#include "MediaServerModel.h"
#include "ServiceProviderDouble.h"
#include <QTest>

namespace Doubles = MediaServerPlugin::Doubles;

namespace MediaServerPlugin
{

MainControllerShould::MainControllerShould() = default;
MainControllerShould::~MainControllerShould() = default;

void MainControllerShould::on_mediaserver_connected_create_media_server_and_put_in_the_model()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    MediaServerModel mediaServerModel;
    MainController mainController;
    Doubles::MediaServerFactory mediaServerFactory;

    mainController.setMediaServerModel(&mediaServerModel);
    mainController.setServiceProviderFactory(&serviceProviderFactory);
    mainController.setMediaServerFactory(&mediaServerFactory);

    mainController.searchMediaServer();

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);
}

void MainControllerShould::handle_every_connected_signal_of_media_server_once()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    MediaServerModel mediaServerModel;
    MainController mainController;
    Doubles::MediaServerFactory mediaServerFactory;

    mainController.setMediaServerModel(&mediaServerModel);
    mainController.setServiceProviderFactory(&serviceProviderFactory);
    mainController.setMediaServerFactory(&mediaServerFactory);

    serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);

    serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);
}

void MainControllerShould::handle_disconnected_media_server()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    MediaServerModel mediaServerModel;
    MainController mainController;
    Doubles::MediaServerFactory mediaServerFactory;

    mainController.setMediaServerModel(&mediaServerModel);
    mainController.setServiceProviderFactory(&serviceProviderFactory);
    mainController.setMediaServerFactory(&mediaServerFactory);

    serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);

    serviceProviderFactory.serviceProvider->serviceDisconnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 0);
}

} // namespace MediaServerPlugin

QTEST_MAIN(MediaServerPlugin::MainControllerShould)
