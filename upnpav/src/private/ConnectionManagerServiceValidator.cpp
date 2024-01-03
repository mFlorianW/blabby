// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ConnectionManagerServiceValidator.hpp"
#include "ScpdActionListValidator.hpp"
#include "ScpdStateVariableValidator.hpp"
#include "ServiceDescriptionValidator.hpp"

namespace UPnPAV
{

ConnectionManagerServiceValidator::ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription)
{
    m_serviceName = "ConnectionManager";
    m_serviceType = "urn:schemas-upnp-org:service:ConnectionManager";
    m_deviceDescription = deviceDescription;

    m_stateVariables = QVector<QString>{"SourceProtocolInfo",
                                        "SinkProtocolInfo",
                                        "CurrentConnectionIDs",
                                        "A_ARG_TYPE_ConnectionStatus",
                                        "A_ARG_TYPE_ConnectionManager",
                                        "A_ARG_TYPE_Direction",
                                        "A_ARG_TYPE_ProtocolInfo",
                                        "A_ARG_TYPE_ConnectionID",
                                        "A_ARG_TYPE_RcsID"};

    m_actions = QVector<QString>{"GetProtocolInfo", "GetCurrentConnectionIDs", "GetCurrentConnectionInfo"};
}

} // namespace UPnPAV
