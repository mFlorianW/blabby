// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RenderingControlStateVariables.hpp"

namespace UPnPAV
{
SCPDStateVariable lastChangeVariable() noexcept
{
    static auto var = SCPDStateVariable{true, QStringLiteral("LastChange"), SCPDStateVariable::String};
    return var;
}

SCPDStateVariable presetNameListVariable() noexcept
{
    static auto var = SCPDStateVariable{false, QStringLiteral("PresetNameList"), SCPDStateVariable::String};
    return var;
}

SCPDStateVariable a_ARG_TYPE_ChannelVariable() noexcept
{
    static auto var = SCPDStateVariable{false,
                                        QStringLiteral("A_ARG_TYPE_Channel"),
                                        SCPDStateVariable::String,
                                        QString{""},
                                        QVector<QString>{
                                            QStringLiteral("Master"),
                                            QStringLiteral("LF"),
                                            QStringLiteral("RF"),
                                            QStringLiteral("CF"),
                                            QStringLiteral("LF"),
                                            QStringLiteral("LS"),
                                            QStringLiteral("RS"),
                                            QStringLiteral("LF"),
                                            QStringLiteral("RF"),
                                            QStringLiteral("SD"),
                                            QStringLiteral("SL"),
                                            QStringLiteral("SR"),
                                            QStringLiteral("T"),
                                            QStringLiteral("B"),
                                            QStringLiteral("Vendor defined"),
                                        }};
    return var;
}

SCPDStateVariable a_ARG_TYPE_InstanceIDVariable() noexcept
{
    static auto var = SCPDStateVariable{
        false,
        QStringLiteral("A_ARG_TYPE_InstanceID"),
        SCPDStateVariable::Ui4,
    };
    return var;
}

SCPDStateVariable a_ARG_TYPE_PresetNameVariable() noexcept
{
    static auto var = SCPDStateVariable{false,
                                        QStringLiteral("A_ARG_TYPE_PresetName"),
                                        SCPDStateVariable::String,
                                        QString{""},
                                        {
                                            QStringLiteral("FactoryDefaults"),
                                            QStringLiteral("InstallationDefaults"),
                                            QStringLiteral("Vendor defined"),
                                        }};
    return var;
}

SCPDStateVariable volume() noexcept
{
    static auto var = SCPDStateVariable{
        false,
        QStringLiteral("Volume"),
        SCPDStateVariable::Ui4,
    };
    return var;
}

} // namespace UPnPAV
