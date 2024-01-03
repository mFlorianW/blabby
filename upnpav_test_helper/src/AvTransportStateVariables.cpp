// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "AvTransportStateVariables.hpp"

namespace UPnPAV
{

SCPDStateVariable createTransportStateVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("TransportState"),
        SCPDStateVariable::DataType::String,
        QString(""),
        {QStringLiteral("STOPPED"), QStringLiteral("PLAYING")}
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createTransportStatusVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("TransportStatus"),
        SCPDStateVariable::DataType::String,
        QString(""),
        {QStringLiteral("OK"), QStringLiteral("ERROR_OCCURED")}
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createPlaybackStorageMediumVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("PlaybackStorageMedium"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createRecordStorageMediumVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("RecordStorageMedium"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createPossiblePlaybackStorageMediaVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("PossiblePlaybackStorageMedia"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createPossibleRecordStorageMediumVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("PossibleRecordStorageMedia"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentPlayModeVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {

        false,
        QStringLiteral("CurrentPlayMode"),
        SCPDStateVariable::DataType::String,
        QStringLiteral("NORMAL"),
        {QStringLiteral("NORMAL")}
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createTransportPlaySpeedVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {

        false,
        QStringLiteral("TransportPlaySpeed"),
        SCPDStateVariable::DataType::String,
        QStringLiteral("1"),
        {QStringLiteral("1")}
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createRecordMediumWriteStatusVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {

        false,
        QStringLiteral("RecordMediumWriteStatus"),
        SCPDStateVariable::DataType::String,
        QString("")
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentRecordQualityModeVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("CurrentRecordQualityMode"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createPossibleRecordQualityModesVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("PossibleRecordQualityModes"),
        SCPDStateVariable::DataType::String,
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createNumberOfTracksVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("NumberOfTracks"),
        SCPDStateVariable::DataType::Ui4,
        QString(""),
        {},
        QStringLiteral("0")
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentTrackVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {

        false,
        QStringLiteral("CurrentTrack"),
        SCPDStateVariable::DataType::Ui4,
        QString(""),
        {},
        QStringLiteral("0"),
        QString(""),
        QStringLiteral("1")
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentTrackDurationVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("CurrentTrackDuration"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentMediaDurationVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("CurrentMediaDuration"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentTrackMetaDataVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("CurrentTrackMetaData"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createCurrentTrackURIVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("CurrentTrackURI"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createAVTransportURIVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("AVTransportURI"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createAVTransportURIMetaDataVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("AVTransportURIMetaData"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createNextAVTransportURIVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("NextAVTransportURI"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createNextAVTransportURIMetaDataVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("NextAVTransportURIMetaData"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createRelativeTimePositionVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("RelativeTimePosition"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createAbsoluteTimePositionVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("AbsoluteTimePosition"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createRelativeCounterPositionVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("RelativeCounterPosition"),
        SCPDStateVariable::DataType::I4
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createAbsoluteCounterPositionVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("AbsoluteCounterPosition"),
        SCPDStateVariable::DataType::I4
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createLastChangeVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("LastChange"),
        SCPDStateVariable::DataType::String

    };
    // clang-format on

    return variable;
}

SCPDStateVariable createA_ARG_TYPE_SeekModeVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("A_ARG_TYPE_SeekMode"),
        SCPDStateVariable::DataType::String,
        QString(""),
        {QStringLiteral("TRACK_NR")}
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createA_ARG_TYPE_SeekTargetVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("A_ARG_TYPE_SeekTarget"),
        SCPDStateVariable::DataType::String
    };
    // clang-format on

    return variable;
}

SCPDStateVariable createA_ARG_TYPE_InstanceIDVariable() noexcept
{
    // clang-format off
    static auto const variable = SCPDStateVariable
    {
        false,
        QStringLiteral("A_ARG_TYPE_InstanceID"),
        SCPDStateVariable::DataType::Ui4
    };
    // clang-format on

    return variable;
}

} // namespace UPnPAV
