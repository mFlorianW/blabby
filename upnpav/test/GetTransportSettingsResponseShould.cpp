// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetTransportSettingsResponseShould.hpp"
#include "AvTransportActions.hpp"
#include "Descriptions.hpp"
#include "GetTransportSettingsResponse.hpp"
#include <QTest>

namespace UPnPAV
{

namespace
{

QString getValidGetTransportSettingsResponse()
{
    constexpr auto validGetTransportSettingsResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
            <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
               <s:Body>
                  <u:GetTransportSettings xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
                    <PlayMode>SHUFFLE</PlayMode>
                    <RecQualityMode>2:SP</RecQualityMode>
                  </u:GetTransportSettings>
               </s:Body>
            </s:Envelope>)";

    return QString{validGetTransportSettingsResponse};
}

} // namespace
GetTransportSettingsResponseShould::~GetTransportSettingsResponseShould() = default;

void GetTransportSettingsResponseShould::give_the_play_mode_of_the_response()
{
    auto resp = GetTransportSettingsResponse{getValidGetTransportSettingsResponse(),
                                             validAvTranportServiceSCPD(),
                                             createGetTransportSettingsAction()};
    auto expPlayMode = GetTransportSettingsResponse::PlayMode::Shuffle;
    auto playMode = resp.playMode();
    QVERIFY2(playMode == expPlayMode,
             QString{"The playModel %1 is not the expected one %2"}
                 .arg(QString::number(static_cast<qint32>(playMode)), QString::number(static_cast<qint32>(expPlayMode)))
                 .toLocal8Bit());
}

void GetTransportSettingsResponseShould::give_the_rec_quality_mode_of_the_response()
{
    auto resp = GetTransportSettingsResponse{getValidGetTransportSettingsResponse(),
                                             validAvTranportServiceSCPD(),
                                             createGetTransportSettingsAction()};
    auto expRecQMode = QStringLiteral("2:SP");
    auto recQMode = resp.recQualityMode();
    QVERIFY2(recQMode == expRecQMode,
             QString{"The playModel %1 is not the expected one %2"}.arg(recQMode, expRecQMode).toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetTransportSettingsResponseShould)
