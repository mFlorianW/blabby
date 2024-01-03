// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "AvTransportActions.hpp"

namespace UPnPAV
{

SCPDAction createSetAVTransportURIAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("SetAVTransportURI"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentURI"),
                SCPDArgument::Direction::In,
                QStringLiteral("AVTransportURI")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentURIMetaData"),
                SCPDArgument::Direction::In,
                QStringLiteral("AVTransportURIMetaData")
            }
        }
    };
    // clang-format on

    return action;
}

SCPDAction createSetNextAVTransportURIAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("SetNextAVTransportURI"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("NextURI"),
                SCPDArgument::Direction::In,
                QStringLiteral("NextAVTransportURI")
            },
            SCPDArgument
            {
                QStringLiteral("NextURIMetaData"),
                SCPDArgument::Direction::In,
                QStringLiteral("NextAVTransportURIMetaData")
            }
        }
    };
    // clang-format on

    return action;
}

SCPDAction createGetMediaInfoAction() noexcept
{

    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("GetMediaInfo"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("NrTracks"),
                SCPDArgument::Direction::Out,
                QStringLiteral("NumberOfTracks")
            },
            SCPDArgument
            {
                QStringLiteral("MediaDuration"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentMediaDuration")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentURI"),
                SCPDArgument::Direction::Out,
                QStringLiteral("AVTransportURI")
            },
            {
                QStringLiteral("CurrentURIMetaData"),
                SCPDArgument::Direction::Out,
                QStringLiteral("AVTransportURIMetaData")
            },
            SCPDArgument
            {
                QStringLiteral("NextURI"),
                SCPDArgument::Direction::Out,
                QStringLiteral("NextAVTransportURI")
            },
            SCPDArgument
            {
                QStringLiteral("NextURIMetaData"),
                SCPDArgument::Direction::Out,
                QStringLiteral("NextAVTransportURIMetaData")
            },
            {
                QStringLiteral("PlayMedium"),
                SCPDArgument::Direction::Out,
                QStringLiteral("PlaybackStorageMedium")
            },
            SCPDArgument
            {
                QStringLiteral("RecordMedium"),
                SCPDArgument::Direction::Out,
                QStringLiteral("RecordStorageMedium")
            },
            SCPDArgument
            {
                QStringLiteral("WriteStatus"),
                SCPDArgument::Direction::Out,
                QStringLiteral("RecordMediumWriteStatus")
            },
        }
    };
    // clang-format on
    return action;
}

SCPDAction createGetTransportInfoAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("GetTransportInfo"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentTransportState"),
                SCPDArgument::Direction::Out,
                QStringLiteral("TransportState")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentTransportStatus"),
                SCPDArgument::Direction::Out,
                QStringLiteral("TransportStatus")
            },
            SCPDArgument
            {
                QStringLiteral("CurrentSpeed"),
                SCPDArgument::Direction::Out,
                QStringLiteral("TransportPlaySpeed")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createGetPositionInfoAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("GetPositionInfo"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("Track"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentTrack")
            },
            SCPDArgument
            {
                QStringLiteral("TrackDuration"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentTrackDuration")
            },
            SCPDArgument
            {
                QStringLiteral("TrackMetaData"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentTrackMetaData")
            },
            SCPDArgument
            {
                QStringLiteral("TrackURI"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentTrackURI")
            },
            SCPDArgument
            {
                QStringLiteral("RelTime"),
                SCPDArgument::Direction::Out,
                QStringLiteral("RelativeTimePosition")
            },
            SCPDArgument
            {
                QStringLiteral("AbsTime"),
                SCPDArgument::Direction::Out,
                QStringLiteral("AbsoluteTimePosition")
            },
            SCPDArgument
            {
                QStringLiteral("RelCount"),
                SCPDArgument::Direction::Out,
                QStringLiteral("RelativeCounterPosition")
            },
            SCPDArgument
            {
                QStringLiteral("AbsCount"),
                SCPDArgument::Direction::Out,
                QStringLiteral("AbsoluteCounterPosition")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createGetDeviceCapabilitiesAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("GetDeviceCapabilities"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("PlayMedia"),
                SCPDArgument::Direction::Out,
                QStringLiteral("PossiblePlaybackStorageMedia")
            },
            SCPDArgument
            {
                QStringLiteral("RecMedia"),
                SCPDArgument::Direction::Out,
                QStringLiteral("PossibleRecordStorageMedia")
            },
            SCPDArgument
            {
                QStringLiteral("RecQualityModes"),
                SCPDArgument::Direction::Out,
                QStringLiteral("PossibleRecordQualityModes")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createGetTransportSettingsAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("GetTransportSettings"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("PlayMode"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentPlayMode")
            },
            SCPDArgument
            {
                QStringLiteral("RecQualityMode"),
                SCPDArgument::Direction::Out,
                QStringLiteral("CurrentRecordQualityMode")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createStopAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("Stop"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createPlayAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("Play"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("Speed"),
                SCPDArgument::Direction::In,
                QStringLiteral("TransportPlaySpeed")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createSeekAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("Seek"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
            SCPDArgument
            {
                QStringLiteral("Unit"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_SeekMode")
            },
            SCPDArgument
            {
                QStringLiteral("Target"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_SeekTarget")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createNextAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("Next"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
        }
    };
    // clang-format on

    return action;
}

SCPDAction createPreviousAction() noexcept
{
    // clang-format off
    static auto const action = SCPDAction
    {
        QStringLiteral("Previous"),
        {
            SCPDArgument
            {
                QStringLiteral("InstanceID"),
                SCPDArgument::Direction::In,
                QStringLiteral("A_ARG_TYPE_INSTANCE_ID")
            },
        }
    };
    // clang-format on

    return action;
}

} // namespace UPnPAV
