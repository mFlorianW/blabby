// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetMuteResponseShould.hpp"
#include "Descriptions.hpp"
#include "GetMuteResponse.hpp"
#include "MuteResponse.hpp"
#include "RenderingControlActions.hpp"
#include <QTest>

namespace UPnPAV
{

GetMuteResponseShould::~GetMuteResponseShould() = default;

void GetMuteResponseShould::give_the_mute_of_the_response_data()
{
    QTest::addColumn<QString>("currentMute");
    QTest::addColumn<bool>("expectedMute");

    QTest::newRow("1") << "1" << true;
    QTest::newRow("0") << "0" << false;
    QTest::newRow("true") << "true" << true;
    QTest::newRow("false") << "false" << false;
    QTest::newRow("yes") << "yes" << true;
    QTest::newRow("no") << "no" << false;
}

void GetMuteResponseShould::give_the_mute_of_the_response()
{
    QFETCH(QString, currentMute);
    QFETCH(bool, expectedMute);

    auto const response = GetMuteResponse{getMuteResponse(currentMute), validRenderingControlSCPD(), getMuteAction()};

    QCOMPARE(response.mute(), expectedMute);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetMuteResponseShould)
