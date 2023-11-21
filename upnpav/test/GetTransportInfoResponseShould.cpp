// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetTransportInfoResponseShould.hpp"
#include "AvTransportActions.hpp"
#include "Descriptions.hpp"
#include "GetTransportInfoResponse.hpp"
#include <QTest>

namespace UPnPAV
{

namespace
{

QString createGetTransportInfoResponse()
{
    constexpr auto validGetTransportInfoResponse = R"(<?xml version="1.0" encoding="UTF-8"?>
            <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
               <s:Body>
                  <u:GetCurrentConnectionInfoResponse xmlns:u="urn:schemas-upnp-org:service:ConnectionManager:1">
                    <CurrentTransportState>PLAYING</CurrentTransportState>
                    <CurrentTransportStatus>ERROR_OCCURED</CurrentTransportStatus>
                    <CurrentSpeed>1/2</CurrentSpeed>
                  </u:GetCurrentConnectionInfoResponse>
               </s:Body>
            </s:Envelope>)";
    return {validGetTransportInfoResponse};
}

} // namespace

GetTransportInfoResponseShould::~GetTransportInfoResponseShould() = default;

void GetTransportInfoResponseShould::give_the_current_transport_state_of_a_response()
{
    auto resp = GetTransportInfoResponse{createGetTransportInfoResponse(),
                                         validAvTranportServiceSCPD(),
                                         createGetTransportInfoAction()};
    const auto expectedTransportState = GetTransportInfoResponse::TransportState::Playing;

    const auto currentTransportState = resp.currentTransportState();

    QVERIFY2(currentTransportState == expectedTransportState,
             QString("The currentTransportState %1 is not the expected %2")
                 .arg(QString::number(static_cast<quint32>(currentTransportState)),
                      QString::number(static_cast<quint32>(expectedTransportState)))
                 .toLocal8Bit());
}

void GetTransportInfoResponseShould::give_the_current_transport_status_of_a_response()
{
    auto resp = GetTransportInfoResponse{createGetTransportInfoResponse(),
                                         validAvTranportServiceSCPD(),
                                         createGetTransportInfoAction()};
    const auto expectedTransportStatus = GetTransportInfoResponse::TransportStatus::ErrorOccured;

    const auto currentTransportStatus = resp.currentTransportStatus();

    QVERIFY2(currentTransportStatus == expectedTransportStatus,
             QString("The currentTransportStatus %1 is not the expected %2")
                 .arg(QString::number(static_cast<quint32>(currentTransportStatus)),
                      QString::number(static_cast<quint32>(expectedTransportStatus)))
                 .toLocal8Bit());
}

void GetTransportInfoResponseShould::give_the_transport_play_speed_of_a_response()
{
    auto resp = GetTransportInfoResponse{createGetTransportInfoResponse(),
                                         validAvTranportServiceSCPD(),
                                         createGetTransportInfoAction()};
    const auto expectedTransportPlaySpeed = QStringLiteral("1/2");

    const auto transportPlaySpeed = resp.transportPlaySpeed();

    QVERIFY2(transportPlaySpeed == expectedTransportPlaySpeed,
             QString("The transportPlaySpeed %1 is not the expected %2")
                 .arg(transportPlaySpeed, expectedTransportPlaySpeed)
                 .toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetTransportInfoResponseShould);
