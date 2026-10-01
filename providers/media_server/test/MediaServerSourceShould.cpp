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

QString didlWithItems(QStringList const& titles)
{
    auto items = QString{};
    for (auto const& title : titles) {
        items += QStringLiteral("&lt;item id=&quot;%1&quot; parentID=&quot;12&quot; restricted=&quot;1&quot;&gt;"
                                "&lt;dc:title&gt;%1&lt;/dc:title&gt;"
                                "&lt;upnp:class&gt;object.item.audioItem.musicTrack&lt;/upnp:class&gt;"
                                "&lt;/item&gt;")
                     .arg(title);
    }
    return QStringLiteral("&lt;DIDL-Lite xmlns:dc=&quot;http://purl.org/dc/elements/1.1/&quot; "
                          "xmlns:upnp=&quot;urn:schemas-upnp-org:metadata-1-0/upnp/&quot; "
                          "xmlns=&quot;urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/&quot;&gt;"
                          "%1"
                          "&lt;/DIDL-Lite&gt;")
        .arg(items);
}

/**
 * Lets the pending Browse of the MediaServer return the Items with the titles out of the total number of Items.
 */
void finishBrowse(UPnPAV::Doubles::MediaServer& mediaServer, QStringList const& titles, qsizetype totalMatches)
{
    mediaServer.soapCall->setRawMessage(QString{UPnPAV::xmlResponse}.arg(didlWithItems(titles),
                                                                         QString::number(titles.size()),
                                                                         QString::number(totalMatches),
                                                                         QStringLiteral("1")));
    Q_EMIT mediaServer.soapCall->finished();
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

void SourceShould::request_the_first_page_of_a_container()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    QCOMPARE(mediaServerRaw->lastBrowseStartingIndex, 0);
    QCOMPARE(mediaServerRaw->lastBrowseRequestedCount, 100);
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 1);

    mediaServerSource.navigateTo(QStringLiteral("12"));

    QCOMPARE(mediaServerRaw->lastBrowseRequest.objectId, QStringLiteral("12"));
    QCOMPARE(mediaServerRaw->lastBrowseStartingIndex, 0);
    QCOMPARE(mediaServerRaw->lastBrowseRequestedCount, 100);
}

void SourceShould::give_the_total_item_count_of_the_container()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};

    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1"), QStringLiteral("Song 2")}, 250);

    QCOMPARE(mediaServerSource.mediaItems().size(), 2);
    QCOMPARE(mediaServerSource.totalItemCount(), 250);
    QCOMPARE(mediaServerSource.canLoadMore(), true);
}

void SourceShould::request_the_next_page_of_the_current_container()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Music")}, 1);
    mediaServerSource.navigateTo(QStringLiteral("12"));
    // A capped MediaServer returns fewer Items than requested, the next page starts after the loaded ones.
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1"), QStringLiteral("Song 2")}, 250);

    mediaServerSource.loadMore();

    QCOMPARE(mediaServerRaw->lastBrowseRequest.objectId, QStringLiteral("12"));
    QCOMPARE(mediaServerRaw->lastBrowseRequest.browseFlag, UPnPAV::MediaServer::BrowseFlag::DirectChildren);
    QCOMPARE(mediaServerRaw->lastBrowseStartingIndex, 2);
    QCOMPARE(mediaServerRaw->lastBrowseRequestedCount, 100);
}

void SourceShould::append_the_items_of_the_next_page()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1"), QStringLiteral("Song 2")}, 4);
    auto const navFinishedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFinished};
    auto const moreLoadedSpy = QSignalSpy{&mediaServerSource, &Source::moreItemsLoaded};

    mediaServerSource.loadMore();
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 3"), QStringLiteral("Song 4")}, 4);

    QCOMPARE(moreLoadedSpy.size(), 1);
    QCOMPARE(navFinishedSpy.size(), 0);
    QCOMPARE(mediaServerSource.mediaItems().size(), 4);
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), QStringLiteral("Song 1"));
    QCOMPARE(mediaServerSource.mediaItems().at(3).mainText(), QStringLiteral("Song 4"));
    QCOMPARE(mediaServerSource.canLoadMore(), false);
}

void SourceShould::take_the_latest_total_item_count()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 10);

    mediaServerSource.loadMore();
    // The Container shrank between the pages.
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 2")}, 2);

    QCOMPARE(mediaServerSource.totalItemCount(), 2);
    QCOMPARE(mediaServerSource.canLoadMore(), false);
}

void SourceShould::end_loading_on_a_page_without_items()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 10);
    auto const moreLoadedSpy = QSignalSpy{&mediaServerSource, &Source::moreItemsLoaded};

    mediaServerSource.loadMore();
    finishBrowse(*mediaServerRaw, {}, 10);

    QCOMPARE(moreLoadedSpy.size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.totalItemCount(), 1);
    QCOMPARE(mediaServerSource.canLoadMore(), false);
}

void SourceShould::report_a_failed_page_and_keep_the_items()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 10);
    auto const moreLoadedSpy = QSignalSpy{&mediaServerSource, &Source::moreItemsLoaded};
    auto const loadingMoreFailedSpy = QSignalSpy{&mediaServerSource, &Source::loadingMoreFailed};
    auto const navFailedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFailed};

    mediaServerSource.loadMore();
    mediaServerRaw->soapCall->setErrorState(true);
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 2")}, 10);

    QCOMPARE(loadingMoreFailedSpy.size(), 1);
    QCOMPARE(moreLoadedSpy.size(), 0);
    QCOMPARE(navFailedSpy.size(), 0);
    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.totalItemCount(), 10);
    QCOMPARE(mediaServerSource.canLoadMore(), true);
}

void SourceShould::not_load_more_when_every_item_is_loaded()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 1);
    auto const browseCount = mediaServerRaw->browseCount;

    mediaServerSource.loadMore();

    QCOMPARE(mediaServerRaw->browseCount, browseCount);
}

void SourceShould::not_load_more_while_browsing()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 10);
    mediaServerSource.loadMore();
    auto const browseCount = mediaServerRaw->browseCount;

    mediaServerSource.loadMore();

    QCOMPARE(mediaServerRaw->browseCount, browseCount);
}

void SourceShould::navigate_instead_of_loading_more_when_navigating_meanwhile()
{
    auto mediaServer = createMediaServer();
    auto mediaServerRaw = mediaServer.get();
    auto mediaServerSource = Source{std::move(mediaServer)};
    finishBrowse(*mediaServerRaw, {QStringLiteral("Song 1")}, 10);
    auto const navFinishedSpy = QSignalSpy{&mediaServerSource, &Source::navigationFinished};
    auto const moreLoadedSpy = QSignalSpy{&mediaServerSource, &Source::moreItemsLoaded};
    mediaServerSource.loadMore();

    mediaServerSource.navigateTo(QStringLiteral("12"));
    finishBrowse(*mediaServerRaw, {QStringLiteral("Album 1")}, 1);

    QCOMPARE(navFinishedSpy.size(), 1);
    QCOMPARE(moreLoadedSpy.size(), 0);
    QCOMPARE(mediaServerRaw->lastBrowseStartingIndex, 0);
    QCOMPARE(mediaServerSource.mediaItems().size(), 1);
    QCOMPARE(mediaServerSource.mediaItems().at(0).mainText(), QStringLiteral("Album 1"));
    QCOMPARE(mediaServerSource.canLoadMore(), false);
}

} // namespace Provider::MediaServer

QTEST_MAIN(Provider::MediaServer::SourceShould);
