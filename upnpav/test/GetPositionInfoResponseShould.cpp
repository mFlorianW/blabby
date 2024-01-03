// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetPositionInfoResponseShould.hpp"
#include "AvTransportActions.hpp"
#include "Descriptions.hpp"
#include "GetPositionInfoResponse.hpp"
#include <QTest>

namespace UPnPAV
{

namespace
{

QString getValidPositionInfoResponse()
{
    constexpr auto validGetMediaInfoResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
            <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
               <s:Body>
                  <u:GetPositionInfo xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
                    <Track>3</Track>
                    <TrackDuration>1:25:19.532</TrackDuration>
                    <TrackMetaData>NOT_IMPLEMENTED</TrackMetaData>
                    <TrackURI>http://world.com/banger.mp3</TrackURI>
                    <RelTime>0:23:53.123</RelTime>
                    <AbsTime>01:23:53.123</AbsTime>
                    <RelCount>5</RelCount>
                    <AbsCount>12</AbsCount>
                  </u:GetPositionInfo>
               </s:Body>
            </s:Envelope>)";
    return {validGetMediaInfoResponse};
}
} // namespace

void GetPositionInfoResponseShould::give_the_track_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expCurrentTrack = quint32{3};
    auto currentTrack = resp.track();
    QVERIFY2(currentTrack == expCurrentTrack,
             QString{"The track %1 is not the same as the expected %2"}
                 .arg(QString::number(currentTrack), QString::number(expCurrentTrack))
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_track_duration_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expCurrentTrackDuration = QTime{1, 25, 19, 532};
    auto trackDuration = resp.trackDuration();
    QVERIFY2(trackDuration == expCurrentTrackDuration,
             QString{"The trackDuration %1 is not the same as the expected %2"}
                 .arg(trackDuration.toString(), expCurrentTrackDuration.toString())
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_track_meta_data_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expTrackMetaData = QStringLiteral("NOT_IMPLEMENTED");
    auto trackMetaData = resp.trackMetaData();
    QVERIFY2(trackMetaData == expTrackMetaData,
             QString{"The trackMetaData %1 is not the same as the expected %2"}
                 .arg(trackMetaData, expTrackMetaData)
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_track_uri_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expTrackUri = QStringLiteral("http://world.com/banger.mp3");
    auto trackUri = resp.trackUri();
    QVERIFY2(trackUri == expTrackUri,
             QString{"The trackUri %1 is not the same as the expected %2"}.arg(trackUri, expTrackUri).toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_rel_time_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expRelTime = QTime{0, 23, 53, 123};
    auto relTime = resp.relTime();
    QVERIFY2(relTime == expRelTime,
             QString{"The relTime %1 is not the same as the expected %2"}
                 .arg(relTime.toString(), expRelTime.toString())
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_abs_time_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expAbsTime = QTime{1, 23, 53, 123};
    auto absTime = resp.absTime();
    QVERIFY2(absTime == expAbsTime,
             QString{"The absTime %1 is not the same as the expected %2"}
                 .arg(absTime.toString(), expAbsTime.toString())
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_rel_count_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expRelCount = qint32{5};
    auto relCount = resp.relCount();
    QVERIFY2(relCount == expRelCount,
             QString{"The relCount %1 is not the same as the expected %2"}
                 .arg(QString::number(relCount), QString::number(expRelCount))
                 .toLocal8Bit());
}

void GetPositionInfoResponseShould::give_the_abs_count_of_the_response()
{
    auto resp = GetPositionInfoResponse{getValidPositionInfoResponse(),
                                        validAvTranportServiceSCPD(),
                                        createGetPositionInfoAction()};
    auto expAbsCount = qint32{12};
    auto absCount = resp.absCount();
    QVERIFY2(absCount == expAbsCount,
             QString{"The relCount %1 is not the same as the expected %2"}
                 .arg(QString::number(absCount), QString::number(expAbsCount))
                 .toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetPositionInfoResponseShould)
