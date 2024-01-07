// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetCurrentConnectionIdsResponseShould.hpp"
#include "ConnectionManagerActions.hpp"
#include "CurrentConnectionIdsResponse.hpp"
#include "Descriptions.hpp"
#include "GetCurrentConnectionIdsResponse.hpp"
#include <QTest>
#include <qtestcase.h>

namespace UPnPAV
{

void GetCurrentConnectionIDsResponseShould::give_the_correct_ids_from_response() noexcept
{
    auto const response = GetCurrentConnectionIdsResponse(ValidCurrentConnectionIdsResponse,
                                                          validConnectionManagerSCPD(),
                                                          GetCurrentConnectionIDs());
    auto const expectedResult = QVector<quint32>{12, 13, 52, 12, 16};

    QVERIFY2(response.currentConnectionIds() == expectedResult,
             "The returned connection ids doesn't match the expected result");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetCurrentConnectionIDsResponseShould)
