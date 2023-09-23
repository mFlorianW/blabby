// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "CurrentConnectionIdsResultShould.h"
#include "CurrentConnectionIdsResponse.h"
#include "CurrentConnectionIdsResult.h"
#include <QTest>
#include <qtestcase.h>

namespace UPnPAV
{

void CurrentConnectionIDsResultShould::give_the_correct_ids_from_response() noexcept
{
    const auto response = CurrentConnectionIdsResult(ValidCurrentConnectionIdsResponse);
    const auto expectedResult = QVector<quint32>{12, 13, 52, 12, 16};

    QVERIFY2(response.currentConnectionIds() == expectedResult,
             "The returned connection ids doesn't match the expected result");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::CurrentConnectionIDsResultShould)
