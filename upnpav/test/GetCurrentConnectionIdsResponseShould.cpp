// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetCurrentConnectionIdsResponseShould.h"
#include "ConnectionManagerActions.h"
#include "CurrentConnectionIdsResponse.h"
#include "Descriptions.h"
#include "GetCurrentConnectionIdsResponse.h"
#include <QTest>
#include <qtestcase.h>

namespace UPnPAV
{

void GetCurrentConnectionIDsResponseShould::give_the_correct_ids_from_response() noexcept
{
    const auto response = GetCurrentConnectionIdsResponse(ValidCurrentConnectionIdsResponse,
                                                          validConnectionManagerSCPD,
                                                          GetCurrentConnectionIDs);
    const auto expectedResult = QVector<quint32>{12, 13, 52, 12, 16};

    QVERIFY2(response.currentConnectionIds() == expectedResult,
             "The returned connection ids doesn't match the expected result");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetCurrentConnectionIDsResponseShould)
