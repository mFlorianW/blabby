// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "RenderingControlServiceValidator.hpp"

namespace UPnPAV
{

RenderingControlServiceValidator::RenderingControlServiceValidator(DeviceDescription const &desc)
{
    m_serviceName = QStringLiteral("RenderingControl");
    m_deviceDescription = desc;
    m_serviceType = QStringLiteral("urn:schemas-upnp-org:service:RenderingControl");

    // Ignore actions and state variables because not every device is implementing
    // the required functions.
    // m_stateVariables = QVector<QString>{
    //     QStringLiteral("LastChange"),
    //     QStringLiteral("PresetNameList"),
    //     QStringLiteral("A_ARG_TYPE_Channel"),
    //     QStringLiteral("A_ARG_TYPE_InstanceID"),
    //     QStringLiteral("A_ARG_TYPE_PresetName"),
    // };

    // m_actions = QVector<QString>{QStringLiteral("ListPresets"), QStringLiteral("SelectPreset")};
}

} // namespace UPnPAV
