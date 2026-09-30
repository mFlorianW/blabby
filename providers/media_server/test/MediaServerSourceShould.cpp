// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
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

QString didlWithOneObject(QString const& element, QString const& typeClass)
{
    return QStringLiteral("&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; "
                          "xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; "
                          "xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
                          "&lt;%1 id=&quot;1&quot; parentID=&quot;0&quot; restricted=&quot;1&quot;&gt;"
                          "&lt;dc:title&gt;Object&lt;/dc:title&gt;"
                          "&lt;upnp:class&gt;%2&lt;/upnp:class&gt;"
                          "&lt;/%1&gt;"
                          "&lt;/DIDL-Lite&gt;")
        .arg(element, typeClass);
}

QString didlWithOneItem(QString const& properties)
{
    return QStringLiteral("&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; "
                          "xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; "
                          "xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
                          "&lt;item id=&quot;1&quot; parentID=&quot;0&quot; restricted=&quot;1&quot;&gt;"
                          "&lt;dc:title&gt;Song&lt;/dc:title&gt;"
                          "&lt;upnp:class&gt;object.item.audioItem.musicTrack&lt;/upnp:class&gt;"
                          "%1"
                          "&lt;/item&gt;"
                          "&lt;/DIDL-Lite&gt;")
        .arg(properties);
}

} // namespace

SourceShould::~SourceShould() = default;

void SourceShould::give_the_name_of_the_media_server()
{
    auto mediaServer = createMediaServer();
    auto mediaServerSource = Source{std::move(mediaServer)};
    auto const expName = QStringLiteral("MediaServer");

    auto const name = mediaServerSource.sourceName();

    QVERIFY2(
        name == expName,
        QStringLiteral("The media server name \"%1\" is not the expected one %2").arg(name, expName).toLocal8Bit());
}

void SourceShould::give_the_icon_of_the_media_server()
{
    auto mediaServer = createMediaServer();
    auto mediaServerSource = Source{std::move(mediaServer)};
    auto const expIcon = QStringLiteral("http://localhost:8200/icons/sm.png");

    auto const icon = mediaServerSource.iconUrl();

    QVERIFY2(
        icon == expIcon,
        QStringLiteral("The media server icon \"%1\" is not the expected one %2").arg(icon, expIcon).toLocal8Bit());
}

void SourceShould::request_root_media_items_on_init()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    auto const expBrowseRequest =
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
    auto const expectedMediaItems = Multimedia::Items{Multimedia::Item{Multimedia::ItemType::Container,
                                                                       QStringLiteral("MyMusic"),
                                                                       QString{""},
                                                                       QString{""},
                                                                       QString{"1"}}};

    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).type(), expectedMediaItems.at(0).type());
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), expectedMediaItems.at(0).mainText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).secondaryText(), expectedMediaItems.at(0).secondaryText());
    QCOMPARE(mediaServerSource.mediaItems().at(0).artworkUrl(), expectedMediaItems.at(0).artworkUrl());
    QCOMPARE(mediaServerSource.mediaItems().at(0).path(), expectedMediaItems.at(0).path());
}

void SourceShould::send_correct_request_on_navigation()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    auto const expBrowseRequest =
        UPnPAV::Doubles::LastBrowseRequest{.objectId = QStringLiteral("12"),
                                           .browseFlag = UPnPAV::MediaServer::BrowseFlag::DirectChildren};
    auto const navFinishedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFinished};

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
    auto const expectedMediaItems = Multimedia::Items{Multimedia::Item{Multimedia::ItemType::Container,
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
    QCOMPARE(mediaServerSource.mediaItems().at(0).artworkUrl(), expectedMediaItems.at(0).artworkUrl());
    QCOMPARE(mediaServerSource.mediaItems().at(0).path(), expectedMediaItems.at(0).path());
}

void SourceShould::give_a_default_icon_when_no_icon_is_set()
{
    auto mediaServer = createMediaServer();
    mediaServer->setIconUrl(QString(""));
    auto mediaServerSource = Source{std::move(mediaServer)};
    auto const expIconUrl = QStringLiteral("qrc:/mediaserverprovider/icons/24x24/PC.svg");

    QCOMPARE(mediaServerSource.iconUrl(), expIconUrl);
}

void SourceShould::classify_objects_by_their_class_data()
{
    QTest::addColumn<QString>("element");
    QTest::addColumn<QString>("typeClass");
    QTest::addColumn<Multimedia::ItemType>("expectedType");

    auto const container = QStringLiteral("container");
    auto const item = QStringLiteral("item");
    QTest::newRow("container") << container << QStringLiteral("object.container") << Multimedia::ItemType::Container;
    QTest::newRow("storage folder") << container << QStringLiteral("object.container.storageFolder")
                                    << Multimedia::ItemType::Container;
    QTest::newRow("album") << container << QStringLiteral("object.container.album.musicAlbum")
                           << Multimedia::ItemType::Container;
    QTest::newRow("artist") << container << QStringLiteral("object.container.person.musicArtist")
                            << Multimedia::ItemType::Container;
    QTest::newRow("genre") << container << QStringLiteral("object.container.genre.musicGenre")
                           << Multimedia::ItemType::Container;
    QTest::newRow("playlist") << container << QStringLiteral("object.container.playlistContainer")
                              << Multimedia::ItemType::Container;
    QTest::newRow("music track") << item << QStringLiteral("object.item.audioItem.musicTrack")
                                 << Multimedia::ItemType::Playable;
    QTest::newRow("video") << item << QStringLiteral("object.item.videoItem.movie") << Multimedia::ItemType::Playable;
    QTest::newRow("item mentioning a storage folder")
        << item << QStringLiteral("object.item.storageFolder") << Multimedia::ItemType::Playable;
}

void SourceShould::classify_objects_by_their_class()
{
    QFETCH(QString, element);
    QFETCH(QString, typeClass);
    QFETCH(Multimedia::ItemType, expectedType);
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    mediaServer->soapCall->setRawMessage(
        QString{UPnPAV::xmlResponse}.arg(didlWithOneObject(element, typeClass), "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};

    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).type(), expectedType);
}

void SourceShould::request_the_album_art_the_artist_and_the_creator()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();

    auto mediaServerSource = Source{std::move(mediaServer)};

    auto const filter = mediaServerRaw->lastBrowseFilter.split(QLatin1Char(','));
    QVERIFY(filter.contains(QStringLiteral("res")));
    QVERIFY(filter.contains(QStringLiteral("upnp:albumArtURI")));
    QVERIFY(filter.contains(QStringLiteral("upnp:artist")));
    QVERIFY(filter.contains(QStringLiteral("dc:creator")));
}

void SourceShould::map_the_album_art_and_the_artist_to_the_item_data()
{
    QTest::addColumn<QString>("properties");
    QTest::addColumn<QString>("expectedArtworkUrl");
    QTest::addColumn<QString>("expectedSecondaryText");

    QTest::newRow("album art, artist and creator")
        << QStringLiteral("&lt;dc:creator&gt;Creator&lt;/dc:creator&gt;"
                          "&lt;upnp:artist&gt;Artist&lt;/upnp:artist&gt;"
                          "&lt;upnp:albumArtURI&gt;http://nas/art.jpg&lt;/upnp:albumArtURI&gt;")
        << QStringLiteral("http://nas/art.jpg") << QStringLiteral("Artist");
    QTest::newRow("only artist") << QStringLiteral("&lt;upnp:artist&gt;Artist&lt;/upnp:artist&gt;") << QString{}
                                 << QStringLiteral("Artist");
    QTest::newRow("only creator") << QStringLiteral("&lt;dc:creator&gt;Creator&lt;/dc:creator&gt;") << QString{}
                                  << QStringLiteral("Creator");
    QTest::newRow("nothing") << QString{} << QString{} << QString{};
}

void SourceShould::map_the_album_art_and_the_artist_to_the_item()
{
    QFETCH(QString, properties);
    QFETCH(QString, expectedArtworkUrl);
    QFETCH(QString, expectedSecondaryText);
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    mediaServer->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(didlWithOneItem(properties), "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};

    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).artworkUrl(), expectedArtworkUrl);
    QCOMPARE(mediaServerSource.mediaItems().at(0).secondaryText(), expectedSecondaryText);
}

void SourceShould::request_the_album_and_the_duration()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();

    auto mediaServerSource = Source{std::move(mediaServer)};

    auto const filter = mediaServerRaw->lastBrowseFilter.split(QLatin1Char(','));
    QVERIFY(filter.contains(QStringLiteral("upnp:album")));
    QVERIFY(filter.contains(QStringLiteral("res@duration")));
}

void SourceShould::map_the_album_and_the_duration_of_the_played_resource_to_the_item()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto const properties =
        QStringLiteral("&lt;upnp:album&gt;Low Tide Sessions&lt;/upnp:album&gt;"
                       "&lt;res protocolInfo=&quot;http-get:*:audio/mpeg:*&quot; duration=&quot;0:04:30&quot;&gt;"
                       "http://nas/1.mp3&lt;/res&gt;"
                       "&lt;res protocolInfo=&quot;http-get:*:audio/flac:*&quot; duration=&quot;0:04:31.250&quot;&gt;"
                       "http://nas/1.flac&lt;/res&gt;");
    mediaServer->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(didlWithOneItem(properties), "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};

    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    auto const& item = mediaServerSource.mediaItems().at(0);
    QCOMPARE(item.album(), QStringLiteral("Low Tide Sessions"));
    QCOMPARE(item.playUrl(), QStringLiteral("http://nas/1.flac"));
    QCOMPARE(item.duration(), std::optional{std::chrono::milliseconds{271250}});
}

void SourceShould::report_a_failed_browse_and_keep_the_items()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    mediaServer->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(UPnPAV::didlOnlyOneContainer, "1", "1", "1"));
    auto mediaServerSource = Source{std::move(mediaServer)};
    Q_EMIT mediaServerRaw->soapCall->finished();
    auto const navFinishedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFinished};
    auto const navFailedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFailed};

    mediaServerSource.navigateTo(QStringLiteral("12"));
    // The failed Browse carries a valid DIDL result, which must not replace the Items.
    mediaServerRaw->soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(UPnPAV::didlOnlyOneItem, "1", "1", "1"));
    mediaServerRaw->soapCall->setErrorState(true);
    Q_EMIT mediaServerRaw->soapCall->finished();

    QCOMPARE(navFinishedSpy.size(), 0);
    QCOMPARE(navFailedSpy.size(), 1);
    QCOMPARE(navFailedSpy.at(0).at(0).toString(), QStringLiteral("12"));
    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), QStringLiteral("MyMusic"));
}

} // namespace Provider::MediaServer

QTEST_MAIN(Provider::MediaServer::SourceShould);
