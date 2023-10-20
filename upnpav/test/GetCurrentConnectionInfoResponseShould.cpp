// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetCurrentConnectionInfoResponseShould.h"
#include "ConnectionManagerActions.h"
#include "CurrentConnectionInfoResponse.h"
#include "Descriptions.h"
#include "GetCurrentConnectionInfoResponse.h"
#include <QTest>

namespace UPnPAV
{

void GetCurrentConnetionInfoResponseShould::gives_the_correct_result_for_valid_response()
{
    const auto respone = GetCurrentConnectionInfoResponse(ValidCurrentConnectionInfoResponse,
                                                          validConnectionManagerSCPD(),
                                                          GetCurrentConnectionInfo);
    const auto expectedInfo = ConnectionInfo{
        .rcsId = 123,
        .avTransportId = 12,
        .protoclInfo = "audio/mpeg",
        .peerConnectionManager = "DummyText",
        .peerConnectionId = 321,
        .direction = ConnectionInfoDirection::Input,
        .status = ConnectionInfoStatus::OK,
    };

    QVERIFY2(respone.connectionInfo() == expectedInfo, "The returned connection info doesn't fit the expected one.");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetCurrentConnetionInfoResponseShould)
