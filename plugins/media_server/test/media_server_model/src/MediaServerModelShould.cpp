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
#include "MediaServerModelShould.h"
#include "IMediaServer.h"
#include "MediaServerDouble.h"
#include "MediaServerModel.h"
#include "PendingSoapCall.h"
#include <QtTest>

namespace Doubles = MediaServerPlugin::Doubles;

namespace MediaServerPlugin
{

MediaServerModelShould::MediaServerModelShould()
    : QObject()
{
}

void MediaServerModelShould::give_correct_rolename()
{
    QHash<int, QByteArray> expectedRoleNames{ std::make_pair(MediaServerModel::MediaServerName, "mediaServerName"),
                                              std::make_pair(MediaServerModel::MediaServerIconUrl,
                                                             "mediaServerIconUrl") };
    MediaServerModel mediaServerModel;

    auto roleNames = mediaServerModel.roleNames();

    QCOMPARE(roleNames, expectedRoleNames);
}

void MediaServerModelShould::give_mediaserver_name_on_request_with_valid_index()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QString expected{ "MediaServer" };

    auto mediaServerName = mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerName).toString();

    QCOMPARE(mediaServerName, expected);
}

void MediaServerModelShould::give_mediaserver_icon_url_on_request_with_valid_index()
{
    Doubles::MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QUrl expected{ "http://localhost:8200/icons/sm.png" };

    auto mediaServerName = mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerIconUrl).toUrl();

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

} // namespace MediaServerPlugin

QTEST_MAIN(MediaServerPlugin::MediaServerModelShould)
