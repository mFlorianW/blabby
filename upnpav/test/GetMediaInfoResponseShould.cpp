// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetMediaInfoResponseShould.hpp"
#include "AvTransportActions.hpp"
#include "Descriptions.hpp"
#include "GetMediaInfoResponse.hpp"
#include <QTest>

namespace UPnPAV
{

namespace
{

QString getValidGetMediaInfoResponse()
{
    constexpr auto validGetMediaInfoResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
            <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
               <s:Body>
                  <u:GetCurrentConnectionInfoResponse xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
                     <RcsID>123</RcsID>
                     <NrTracks>12</NrTracks>
                     <MediaDuration>01:23:23.123</MediaDuration>
                     <CurrentURI>http://myworld.com/music.mp3</CurrentURI>
                     <CurrentURIMetaData>NOT_IMPLEMENTED</CurrentURIMetaData>
                     <NextURI>http://myworld.com/music2.mp3</NextURI>
                     <NextURIMetaData>NOT_IMPLEMENTED</NextURIMetaData>
                     <PlayMedium>NETWORK</PlayMedium>
                     <RecordMedium>HDD</RecordMedium>
                     <WriteStatus>WRITEABLE</WriteStatus>
                  </u:GetCurrentConnectionInfoResponse>
               </s:Body>
            </s:Envelope>)";

    return {validGetMediaInfoResponse};
}

} // namespace

GetMediaInfoResponseShould::~GetMediaInfoResponseShould() = default;

void GetMediaInfoResponseShould::the_get_media_response_shall_return_the_nr_tracks_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    constexpr auto expectedNrTracks = quint32{12};

    const auto nrTracks = resp.numberOfTracks();

    QVERIFY2(nrTracks == expectedNrTracks,
             QString("The numberOfTracks %1 is not the expected %2")
                 .arg(QString::number(nrTracks), QString::number(expectedNrTracks))
                 .toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_media_duration_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedDuration = QTime(1, 23, 23, 123);

    const auto duration = resp.mediaDuration();

    QVERIFY(duration.has_value() == true);
    QVERIFY2(duration.value_or(QTime{}) == expectedDuration,
             QString("The mediaDuration %1 is not the expected %2")
                 .arg(duration.value_or(QTime{}).toString(), expectedDuration.toString())
                 .toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_current_uri_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedCurrentUri = QStringLiteral("http://myworld.com/music.mp3");

    const auto currentUri = resp.currentUri();

    QVERIFY2(currentUri == expectedCurrentUri,
             QString("The currentUri %1 is not the expected %2").arg(currentUri, expectedCurrentUri).toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_current_meta_data_of_a_response()
{

    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedCurrentMetaData = QStringLiteral("NOT_IMPLEMENTED");

    const auto currentMetaData = resp.currentUriMetaData();

    QVERIFY2(currentMetaData == expectedCurrentMetaData,
             QString("The currentUriMetaData %1 is not the expected %2")
                 .arg(currentMetaData, expectedCurrentMetaData)
                 .toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_next_uri_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedNextUri = QStringLiteral("http://myworld.com/music2.mp3");

    const auto nextUri = resp.nextUri();

    QVERIFY2(nextUri == expectedNextUri,
             QString("The nextUri %1 is not the expected %2").arg(nextUri, expectedNextUri).toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_next_uri_meta_data_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedNextUriMetaData = QStringLiteral("NOT_IMPLEMENTED");

    const auto nextUriMetaData = resp.nextUriMetaData();

    QVERIFY2(nextUriMetaData == expectedNextUriMetaData,
             QString("The currentUri %1 is not the expected %2")
                 .arg(nextUriMetaData, expectedNextUriMetaData)
                 .toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_play_medium_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedPlayMedium = QStringLiteral("NETWORK");

    const auto playMedium = resp.playMedium();

    QVERIFY2(playMedium == expectedPlayMedium,
             QString("The currentUri %1 is not the expected %2").arg(playMedium, expectedPlayMedium).toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_record_medium_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedRecordMedium = QStringLiteral("HDD");

    const auto recordMedium = resp.recordMedium();

    QVERIFY2(recordMedium == expectedRecordMedium,
             QString("The currentUri %1 is not the expected %2").arg(recordMedium, expectedRecordMedium).toLocal8Bit());
}

void GetMediaInfoResponseShould::give_the_write_status_of_a_response()
{
    const auto resp =
        GetMediaInfoResponse(getValidGetMediaInfoResponse(), validAvTranportServiceSCPD(), createGetMediaInfoAction());
    auto expectedWriteStatus = QStringLiteral("WRITEABLE");

    const auto writeStatus = resp.writeStatus();

    QVERIFY2(writeStatus == expectedWriteStatus,
             QString("The currentUri %1 is not the expected %2").arg(writeStatus, expectedWriteStatus).toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetMediaInfoResponseShould)
