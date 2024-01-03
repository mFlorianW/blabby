// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerSourceShould.hpp"
#include "ContentDirectoryActions.hpp"
#include "Descriptions.hpp"
#include "MediaServerDouble.hpp"
#include "MediaServerSource.hpp"
#include "Response.hpp"
#include <QSignalSpy>
#include <QTest>

namespace Provider::MediaServer
{

namespace
{

std::unique_ptr<UPnPAV::Doubles::MediaServer> createMediaServer()
{
    auto mediaServer = std::make_unique<UPnPAV::Doubles::MediaServer>();
    auto soapCall =
        QSharedPointer<UPnPAV::SoapCallDouble>::create(UPnPAV::validContentDirectorySCPD(), UPnPAV::Browse());
    mediaServer->soapCall = soapCall;
    return mediaServer;
}

} // namespace

SourceShould::~SourceShould() = default;

void SourceShould::give_the_name_of_the_media_server()
{
    auto mediaServer = createMediaServer();
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expName = QStringLiteral("MediaServer");

    const auto name = mediaServerSource.sourceName();

    QVERIFY2(
        name == expName,
        QStringLiteral("The media server name \"%1\" is not the expected one %2").arg(name, expName).toLocal8Bit());
}

void SourceShould::give_the_icon_of_the_media_server()
{
    auto mediaServer = createMediaServer();
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expIcon = QStringLiteral("http://localhost:8200/icons/sm.png");

    const auto icon = mediaServerSource.iconUrl();

    QVERIFY2(
        icon == expIcon,
        QStringLiteral("The media server icon \"%1\" is not the expected one %2").arg(icon, expIcon).toLocal8Bit());
}

void SourceShould::request_root_media_items_on_init()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expBrowseRequest =
        UPnPAV::Doubles::LastBrowseRequest{.objectId = QStringLiteral("0"),
                                           .browseFlag = UPnPAV::MediaServer::BrowseFlag::DirectChildren};
    QCOMPARE(mediaServerRaw->lastBrowseRequest, expBrowseRequest);
}

void SourceShould::give_root_media_items_on_init()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    mediaServer->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(UPnPAV::didlOnlyOneContainer, "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expectedMediaItems = Multimedia::Items{Multimedia::Item{Multimedia::ItemType::Container,
                                                                       QStringLiteral("MyMusic"),
                                                                       QString{""},
                                                                       QString{""},
                                                                       QString{"1"}}};

    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).type(), expectedMediaItems.at(0).type());
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), expectedMediaItems.at(0).mainText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).secondaryText(), expectedMediaItems.at(0).secondaryText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).iconUrl(), expectedMediaItems.at(0).iconUrl());
    QCOMPARE(mediaServerSource.mediaItems().at(0).path(), expectedMediaItems.at(0).path());
}

void SourceShould::send_correct_request_on_navigation()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expBrowseRequest =
        UPnPAV::Doubles::LastBrowseRequest{.objectId = QStringLiteral("12"),
                                           .browseFlag = UPnPAV::MediaServer::BrowseFlag::DirectChildren};
    const auto navFinishedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFinished};

    mediaServerSource.navigateTo(QStringLiteral("12"));
    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(navFinishedSpy.size(), 1);
    QCOMPARE(mediaServerRaw->lastBrowseRequest, expBrowseRequest);
}

void SourceShould::request_root_media_items_on_navigation()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    mediaServer->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(UPnPAV::didlOnlyOneContainer, "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expectedMediaItems = Multimedia::Items{Multimedia::Item{Multimedia::ItemType::Container,
                                                                       QStringLiteral("MyMusic"),
                                                                       QString{""},
                                                                       QString{""},
                                                                       QString{"1"}}};

    mediaServerSource.navigateTo(QStringLiteral("0"));
    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).type(), expectedMediaItems.at(0).type());
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), expectedMediaItems.at(0).mainText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).secondaryText(), expectedMediaItems.at(0).secondaryText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).iconUrl(), expectedMediaItems.at(0).iconUrl());
    QCOMPARE(mediaServerSource.mediaItems().at(0).path(), expectedMediaItems.at(0).path());
}

void SourceShould::give_a_default_icon_when_no_icon_is_set()
{
    auto mediaServer = createMediaServer();
    mediaServer->setIconUrl(QString(""));
    auto mediaServerSource = Source{std::move(mediaServer)};
    const auto expIconUrl = QStringLiteral("qrc:/mediaserverprovider/icons/24x24/PC.svg");

    QCOMPARE(mediaServerSource.iconUrl(), expIconUrl);
}

} // namespace Provider::MediaServer

QTEST_MAIN(Provider::MediaServer::SourceShould);
