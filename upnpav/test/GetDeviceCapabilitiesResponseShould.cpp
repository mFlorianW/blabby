// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetDeviceCapabilitiesResponseShould.hpp"
#include "AvTransportActions.hpp"
#include "Descriptions.hpp"
#include "GetDeviceCapabilitiesResponse.hpp"
#include <QTest>

namespace UPnPAV
{

namespace
{

QString getValidGetDeviceCapabilitiesResponse()
{
    constexpr auto validGetDeviceCapabilitiesResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
            <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
               <s:Body>
                  <u:GetDeviceCapabilitiesResponse xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
                    <PlayMedia>NONE, NETWORK</PlayMedia>
                    <RecMedia>NETWORK, HDD</RecMedia>
                    <RecQualityModes>0:EP,1:LP,2:SP</RecQualityModes>
                  </u:GetDeviceCapabilitiesResponse>
               </s:Body>
            </s:Envelope>)";

    return {validGetDeviceCapabilitiesResponse};
}

} // namespace

GetDeviceCapabilitiesResponseShould::~GetDeviceCapabilitiesResponseShould() = default;

void GetDeviceCapabilitiesResponseShould::give_the_play_media_of_the_response()
{
    auto resp = GetDeviceCapabilitiesResponse{getValidGetDeviceCapabilitiesResponse(),
                                              validAvTranportServiceSCPD(),
                                              createGetDeviceCapabilitiesAction()};
    auto const expPlayMedia = QStringList{QStringLiteral("NONE"), QStringLiteral("NETWORK")};
    auto const playMedia = resp.playMedia();

    QVERIFY2(playMedia == expPlayMedia,
             QString{"The playMedia %1 value is not the expected one %2"}
                 .arg(playMedia.join(" "), expPlayMedia.join(" "))
                 .toLocal8Bit());
}

void GetDeviceCapabilitiesResponseShould::give_the_rec_media_of_the_response()
{
    auto resp = GetDeviceCapabilitiesResponse{getValidGetDeviceCapabilitiesResponse(),
                                              validAvTranportServiceSCPD(),
                                              createGetDeviceCapabilitiesAction()};
    auto const expRecMedia = QStringList{QStringLiteral("NETWORK"), QStringLiteral("HDD")};
    auto const recMedia = resp.recMedia();

    QVERIFY2(recMedia == expRecMedia,
             QString{"The recMedia %1 value is not the expected one %2"}
                 .arg(recMedia.join(" "), expRecMedia.join(" "))
                 .toLocal8Bit());
}

void GetDeviceCapabilitiesResponseShould::give_the_rec_quality_modes_of_the_response()
{
    auto resp = GetDeviceCapabilitiesResponse{getValidGetDeviceCapabilitiesResponse(),
                                              validAvTranportServiceSCPD(),
                                              createGetDeviceCapabilitiesAction()};
    auto const expRecQModes = QStringList{QStringLiteral("0:EP"), QStringLiteral("1:LP"), QStringLiteral("2:SP")};
    auto const recQModes = resp.recQualityModes();

    QVERIFY2(recQModes == expRecQModes,
             QString{"The recQualityModes \"%1\" value is not the expected one \"%2\""}
                 .arg(recQModes.join(" "), expRecQModes.join(" "))
                 .toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetDeviceCapabilitiesResponseShould)
