// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerModelShould.h"
#include "IMediaServer.h"
#include "MediaServerDouble.h"
#include "MediaServerModel.h"
#include "PendingSoapCall.h"
#include <QSignalSpy>
#include <QtTest>

namespace Doubles = MediaServer::Plugin::Doubles;

namespace MediaServer::Plugin
{

MediaServerModelShould::MediaServerModelShould()
    : QObject()
{
}

void MediaServerModelShould::give_correct_rolename()
{
    QHash<int, QByteArray> expectedRoleNames{
        std::make_pair(MediaServerModel::MediaServerName, "mediaServerName"),
        std::make_pair(MediaServerModel::MediaServerIconUrl, "mediaServerIconUrl")};
    MediaServerModel mediaServerModel;

    auto roleNames = mediaServerModel.roleNames();

    QCOMPARE(roleNames, expectedRoleNames);
}

void MediaServerModelShould::give_mediaserver_name_on_request_with_valid_index()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QString expected{"MediaServer"};

    auto mediaServerName =
        mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerName).toString();

    QCOMPARE(mediaServerName, expected);
}

void MediaServerModelShould::give_mediaserver_icon_url_on_request_with_valid_index()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QUrl expected{"http://localhost:8200/icons/sm.png"};

    auto mediaServerName =
        mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerIconUrl).toUrl();

    QCOMPARE(mediaServerName, expected);
}

void MediaServerModelShould::give_default_qvariant_for_a_to_small_index()
{
    MediaServerModel mediaServerModel;

    auto emptyVariant = mediaServerModel.data(mediaServerModel.index(-1), MediaServerModel::MediaServerName);

    QCOMPARE(emptyVariant, QVariant{});
}

void MediaServerModelShould::give_default_qvariant_for_a_to_big_index()
{
    MediaServerModel mediaServerModel;

    auto emptyVariant = mediaServerModel.data(mediaServerModel.index(19), MediaServerModel::MediaServerName);

    QCOMPARE(emptyVariant, QVariant{});
}

void MediaServerModelShould::do_not_add_media_server_twice()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    QSignalSpy rowsAboutToInsertSpy(&mediaServerModel, &MediaServerModel::rowsAboutToBeInserted);

    mediaServerModel.insert(&mediaServer);
    QCOMPARE(rowsAboutToInsertSpy.size(), 1);

    mediaServerModel.insert(&mediaServer);
    QCOMPARE(rowsAboutToInsertSpy.size(), 1);
}

void MediaServerModelShould::remove_inserted_media_server_from_model()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    QSignalSpy rowsAboutToBeRemovedSpy(&mediaServerModel, &MediaServerModel::rowsAboutToBeRemoved);

    mediaServerModel.removeServer(&mediaServer);

    QCOMPARE(rowsAboutToBeRemovedSpy.size(), 1);
    QCOMPARE(mediaServerModel.rowCount(mediaServerModel.index(0)), 0);
}

void MediaServerModelShould::give_media_server_for_name()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);

    auto mediaServerResult = mediaServerModel.mediaServer(0);

    QCOMPARE(mediaServerResult, &mediaServer);
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::MediaServerModelShould)
