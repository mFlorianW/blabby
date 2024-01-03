// SPDX-FileCopyrightText: 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetProtocolInfoResponseShould.hpp"
#include "ConnectionManagerActions.hpp"
#include "Descriptions.hpp"
#include "GetProtocolInfoResponse.hpp"
#include "ProtocolInfoResponse.hpp"
#include <QTest>
#include <qtestcase.h>
#include <qvector.h>

namespace UPnPAV
{

void GetProtocolInfoResponseShould::give_the_protocol_lists()
{
    auto pInfo = GetProtocolInfoResponse{ValidProtoclInfoResponse};
    auto expectedResult = QVector<Protocol>{
        Protocol{.protocol = "file", .network = "*", .contentFormat = "audio/mpegurl", .additionalInfo = "*"},
        Protocol{.protocol = "x-file-cifs", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-mp3radio", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-playlist", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-queue", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-stream", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
    };

    QVERIFY2(pInfo.sourceProtocols() == expectedResult,
             "The returned result doesn't match expected source protocol result");
    QVERIFY2(pInfo.sinkProtocols() == expectedResult,
             "The returned result doesn't match expected sink protocol result");

    pInfo = GetProtocolInfoResponse{ValidProtoclInfoResponse, validConnectionManagerSCPD(), GetProtocolInfo()};
    QVERIFY2(pInfo.sourceProtocols() == expectedResult,
             "The returned result doesn't match expected source protocol result");
    QVERIFY2(pInfo.sinkProtocols() == expectedResult,
             "The returned result doesn't match expected sink protocol result");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetProtocolInfoResponseShould)
