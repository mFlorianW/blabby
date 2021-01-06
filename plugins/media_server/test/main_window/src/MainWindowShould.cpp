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
#include "MainWindowShould.h"
#include "MainWindow.h"
#include "MediaServerDouble.h"
#include "MediaServerModel.h"
#include "ServiceProviderDouble.h"
#include <QTest>

namespace Doubles = MediaServer::Plugin::Doubles;

namespace MediaServer::Plugin
{

MainWindowShould::MainWindowShould() = default;
MainWindowShould::~MainWindowShould() = default;

void MainWindowShould::on_mediaserver_connected_create_media_server_and_put_in_the_model()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MainWindow mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    mainController.searchMediaServer();

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);
}

void MainWindowShould::handle_every_connected_signal_of_media_server_once()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MainWindow mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);
}

void MainWindowShould::handle_disconnected_media_server()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MainWindow mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);

    Q_EMIT serviceProviderFactory.serviceProvider->serviceDisconnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 0);
}

void MainWindowShould::set_active_media_server()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MainWindow mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    mainController.setActiveMediaServer(0);

    QVERIFY(mainController.activeMediaServer() != nullptr);
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::MainWindowShould)
