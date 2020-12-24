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
#include "ServerItemControllerShould.h"
#include "MediaServerDouble.h"
#include "Response.h"
#include "ServerItemController.h"
#include "ServerItemModel.h"
#include <QTest>

namespace MediaServer::Plugin
{

using namespace MediaServer::Plugin::Doubles;

ServerItemControllerShould::ServerItemControllerShould()
    : QObject()
{
}

void ServerItemControllerShould::on_inserted_media_server_request_root_folder()
{
    auto controller = ServerItemController{};
    auto mediaServer = MediaServer();
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    const auto expectedBrowseRequest = LastBrowseRequest{ .objectId = "0", .browseFlag = MediaServer::DirectChildren };
    soapCall->setRawMessage(didlOnlyOneContainer);
    mediaServer.soapCall = soapCall;

    controller.setMediaServer(&mediaServer);

    QCOMPARE(mediaServer.lastBrowseRequest, expectedBrowseRequest);
}

void ServerItemControllerShould::on_valid_result_received_insert_media_objects_into_server_item_model()
{
    auto controller = ServerItemController{};
    auto serverItemModel = ServerItemModel{};
    auto mediaServer = MediaServer{};
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    soapCall->setRawMessage(QString{ xmlResponse }.arg(didlOnlyOneContainer, "", "", ""));
    mediaServer.soapCall = soapCall;

    controller.setMediaServer(&mediaServer);
    controller.setServerItemModel(&serverItemModel);

    Q_EMIT soapCall->finished();
    QCOMPARE(serverItemModel.rowCount(QModelIndex{}), 1);
}

void ServerItemControllerShould::request_specific_folder_on_media_server()
{
    auto controller = ServerItemController{};
    auto mediaServer = MediaServer();
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    const auto expectedBrowseRequest = LastBrowseRequest{ .objectId = "1", .browseFlag = MediaServer::DirectChildren };
    mediaServer.soapCall = soapCall;
    controller.setMediaServer(&mediaServer);

    controller.requestStorageFolder("1");

    QCOMPARE(mediaServer.lastBrowseRequest, expectedBrowseRequest);
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::ServerItemControllerShould);
