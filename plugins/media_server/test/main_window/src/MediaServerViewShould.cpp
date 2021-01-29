// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerViewShould.h"
#include "MediaServerDouble.h"
#include "MediaServerModel.h"
#include "MediaServerView.h"
#include "ServiceProviderDouble.h"
#include <QTest>

namespace Doubles = MediaServer::Plugin::Doubles;

namespace MediaServer::Plugin
{

MediaServerViewShould::MediaServerViewShould() = default;
MediaServerViewShould::~MediaServerViewShould() = default;

void MediaServerViewShould::on_mediaserver_connected_create_media_server_and_put_in_the_model()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MediaServerView mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    mainController.searchMediaServer();

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);
}

void MediaServerViewShould::handle_every_connected_signal_of_media_server_once()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MediaServerView mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerFactory.howOftenCalled, 1);
}

void MediaServerViewShould::handle_disconnected_media_server()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MediaServerView mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 1);

    Q_EMIT serviceProviderFactory.serviceProvider->serviceDisconnected("mediaServer");

    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 0);
}

void MediaServerViewShould::set_active_media_server()
{
    Doubles::ServiceProviderFactory serviceProviderFactory;
    Doubles::MediaServerFactory mediaServerFactory;
    MediaServerModel mediaServerModel;
    MediaServerView mainController{ &mediaServerModel, &mediaServerFactory, &serviceProviderFactory };

    Q_EMIT serviceProviderFactory.serviceProvider->serviceConnected("mediaServer");

    mainController.setActiveMediaServer(0);

    QVERIFY(mainController.activeMediaServer() != nullptr);
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::MediaServerViewShould)
