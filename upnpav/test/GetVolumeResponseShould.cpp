// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetVolumeResponseShould.hpp"
#include "Descriptions.hpp"
#include "GetVolumeResponse.hpp"
#include "RenderingControlActions.hpp"
#include "VolumeResponse.hpp"
#include <QTest>

namespace UPnPAV
{

void GetVolumeResponseShould::gives_the_correct_volume_for_valid_response()
{
    auto const response =
        GetVolumeResponse(QString{ValidGetVolumeResponse}, validRenderingControlSCPD(), getVolumeAction());
    auto constexpr expVolume = quint32{98};

    QCOMPARE(response.volume(), expVolume);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::GetVolumeResponseShould)
