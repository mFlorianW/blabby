// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RenderingControlActions.hpp"

namespace UPnPAV
{
SCPDAction listPresetsAction() noexcept
{
    static auto action = SCPDAction{QStringLiteral("ListPresets"),
                                    {SCPDArgument{QStringLiteral("InstanceID"),
                                                  SCPDArgument::Direction::In,
                                                  QStringLiteral("A_ARG_TYPE_InstanceID")},
                                     SCPDArgument{QStringLiteral("CurrentPresetNameLists"),
                                                  SCPDArgument::Direction::Out,
                                                  QStringLiteral("PresetNameList")}}

    };
    return action;
}

SCPDAction selectPresetAction() noexcept
{
    static auto action = SCPDAction{QStringLiteral("SelectPreset"),
                                    {SCPDArgument{QStringLiteral("InstanceID"),
                                                  SCPDArgument::Direction::In,
                                                  QStringLiteral("A_ARG_TYPE_InstanceID")},
                                     SCPDArgument{QStringLiteral("PresetName"),
                                                  SCPDArgument::Direction::In,
                                                  QStringLiteral("A_ARG_TYPE_PresetName")}}

    };
    return action;
}

SCPDAction getVolumeAction() noexcept
{
    // clang-format off
    static auto action = SCPDAction{
        QStringLiteral("GetVolume"),
        {
            SCPDArgument{QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_InstanceID")},
            SCPDArgument{QStringLiteral("Channel"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_Channel")},
            SCPDArgument{QStringLiteral("CurrentVolume"),
                SCPDArgument::Direction::Out,
                QStringLiteral("Volume")},
        }
    };
    // clang-format on
    return action;
}

SCPDAction setVolumeAction() noexcept
{
    // clang-format off
    static auto action = SCPDAction{
        QStringLiteral("SetVolume"),
        {
            SCPDArgument{QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_InstanceID")},
            SCPDArgument{QStringLiteral("Channel"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_Channel")},
            SCPDArgument{QStringLiteral("DesiredVolume"),
                SCPDArgument::Direction::In,
                QStringLiteral("Volume")},
        }
    };
    // clang-format on
    return action;
}

SCPDAction getMuteAction() noexcept
{
    // clang-format off
    static auto action = SCPDAction{
        QStringLiteral("GetMute"),
        {
            SCPDArgument{QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_InstanceID")},
            SCPDArgument{QStringLiteral("Channel"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_Channel")},
            SCPDArgument{QStringLiteral("CurrentMute"),
                SCPDArgument::Direction::Out,
                QStringLiteral("Mute")},
        }
    };
    // clang-format on
    return action;
}

SCPDAction setMuteAction() noexcept
{
    // clang-format off
    static auto action = SCPDAction{
        QStringLiteral("SetMute"),
        {
            SCPDArgument{QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_InstanceID")},
            SCPDArgument{QStringLiteral("Channel"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_Channel")},
            SCPDArgument{QStringLiteral("DesiredMute"),
                SCPDArgument::Direction::In,
                QStringLiteral("Mute")},
        }
    };
    // clang-format on
    return action;
}

} // namespace UPnPAV
