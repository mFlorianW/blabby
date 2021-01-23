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
#include "ServerItemViewShould.h"
#include "MediaServerDouble.h"
#include "Response.h"
#include "ServerItemModel.h"
#include "ServerItemView.h"
#include <QTest>

namespace MediaServer::Plugin
{

using namespace MediaServer::Plugin::Doubles;

ServerItemViewShould::ServerItemViewShould()
    : QObject()
{
}

void ServerItemViewShould::on_inserted_media_server_request_root_folder()
{
    auto serverItemModel = ServerItemModel{};
    auto controller = ServerItemView{ &serverItemModel };
    auto mediaServer = MediaServer();
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    const auto expectedBrowseRequest = LastBrowseRequest{ .objectId = "0", .browseFlag = MediaServer::DirectChildren };
    soapCall->setRawMessage(didlOnlyOneContainer);
    mediaServer.soapCall = soapCall;

    controller.setMediaServer(&mediaServer);

    QCOMPARE(mediaServer.lastBrowseRequest, expectedBrowseRequest);
}

void ServerItemViewShould::on_valid_result_received_insert_media_objects_into_server_item_model()
{
    auto serverItemModel = ServerItemModel{};
    auto controller = ServerItemView{ &serverItemModel };
    auto mediaServer = MediaServer{};
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    soapCall->setRawMessage(QString{ xmlResponse }.arg(didlOnlyOneContainer, "", "", ""));
    mediaServer.soapCall = soapCall;

    controller.setMediaServer(&mediaServer);

    Q_EMIT soapCall->finished();
    QCOMPARE(serverItemModel.rowCount(QModelIndex{}), 1);
}

void ServerItemViewShould::request_specific_folder_on_media_server()
{
    auto serverItemModel = ServerItemModel{};
    auto controller = ServerItemView{ &serverItemModel };
    auto mediaServer = MediaServer();
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    const auto expectedBrowseRequest = LastBrowseRequest{ .objectId = "1", .browseFlag = MediaServer::DirectChildren };
    mediaServer.soapCall = soapCall;
    controller.setMediaServer(&mediaServer);

    controller.requestStorageFolder("1");

    QCOMPARE(mediaServer.lastBrowseRequest, expectedBrowseRequest);
}

void ServerItemViewShould::on_valid_response_of_specific_request_fill_objects_into_server_item_model()
{
    auto serverItemModel = ServerItemModel{};
    auto controller = ServerItemView{ &serverItemModel };
    auto mediaServer = MediaServer{};
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    soapCall->setRawMessage(QString{ xmlResponse }.arg(didlOnlyOneContainer, "", "", ""));
    mediaServer.soapCall = soapCall;
    controller.setMediaServer(&mediaServer);

    Q_EMIT soapCall->finished();
    QCOMPARE(serverItemModel.rowCount(QModelIndex{}), 1);

    soapCall->setRawMessage(QString{ xmlResponse }.arg(didlOnlyTwoContainer, "", "", ""));
    controller.requestStorageFolder("1");

    Q_EMIT soapCall->finished();
    QCOMPARE(serverItemModel.rowCount(QModelIndex{}), 2);
}

void ServerItemViewShould::go_to_previous_folder()
{
    auto serverItemModel = ServerItemModel{};
    auto mediaServer = MediaServer{};
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    auto controller = ServerItemView{ &serverItemModel };
    mediaServer.soapCall = soapCall;
    controller.setMediaServer(&mediaServer);

    controller.requestStorageFolder("1");

    auto result = controller.goPreviousFolder();

    QCOMPARE(result, true);
    QCOMPARE(mediaServer.lastBrowseRequest.objectId, "0");
}

void ServerItemViewShould::go_to_back_to_root_folder()
{
    auto serverItemModel = ServerItemModel{};
    auto mediaServer = MediaServer{};
    auto soapCall = QSharedPointer<Doubles::SoapCallDouble>(new SoapCallDouble{});
    auto controller = ServerItemView{ &serverItemModel };
    mediaServer.soapCall = soapCall;
    controller.setMediaServer(&mediaServer);

    controller.requestStorageFolder("1");
    controller.requestStorageFolder("2");

    auto result = controller.goPreviousFolder();
    QCOMPARE(result, true);
    QCOMPARE(mediaServer.lastBrowseRequest.objectId, "1");

    result = controller.goPreviousFolder();

    QCOMPARE(result, true);
    QCOMPARE(mediaServer.lastBrowseRequest.objectId, "0");
}

} // namespace MediaServer::Plugin

QTEST_MAIN(MediaServer::Plugin::ServerItemViewShould);
