// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Descriptions.h"
#include "AvTransportActions.h"
#include "AvTransportStateVariables.h"

namespace UPnPAV
{

QVector<SCPDStateVariable> validAvTransportStateVariables() noexcept
{
    // clang-format off
    static const auto validAvTransportStateVariables = QVector<SCPDStateVariable>
    {
        createTransportStateVariable(),
        createTransportStatusVariable(),
        createPlaybackStorageMediumVariable(),
        createRecordStorageMediumVariable(),
        createPossiblePlaybackStorageMediaVariable(),
        createPossibleRecordStorageMediumVariable(),
        createCurrentPlayModeVariable(),
        createTransportPlaySpeedVariable() ,
        createRecordMediumWriteStatusVariable(),
        createCurrentRecordQualityModeVariable(),
        createPossibleRecordQualityModesVariable(),
        createNumberOfTracksVariable(),
        createCurrentTrackVariable(),
        createCurrentTrackDurationVariable(),
        createCurrentMediaDurationVariable(),
        createCurrentTrackMetaDataVariable(),
        createCurrentTrackURIVariable(),
        createAVTransportURIVariable(),
        createAVTransportURIMetaDataVariable(),
        createNextAVTransportURIVariable(),
        createNextAVTransportURIMetaDataVariable(),
        createRelativeTimePositionVariable(),
        createAbsoluteTimePositionVariable(),
        createRelativeCounterPositionVariable(),
        createAbsoluteCounterPositionVariable(),
        createLastChangeVariable(),
        createA_ARG_TYPE_SeekModeVariable(),
        createA_ARG_TYPE_SeekTargetVariable(),
        createA_ARG_TYPE_InstanceIDVariable(),
    };
    // clang-format on

    return validAvTransportStateVariables;
};

QVector<SCPDAction> validAvTranportActions() noexcept
{
    // clang-format off
    static const auto validAvTransportActions = QVector<SCPDAction>
    {
        createSetAVTransportURIAction(),
        createGetMediaInfoAction(),
        createGetTransportInfoAction(),
        createGetPositionInfoAction(),
        createGetDeviceCapabilitiesAction(),
        createGetTransportSettingsAction(),
        createStopAction(),
        createPlayAction(),
        createSeekAction(),
        createNextAction(),
        createPreviousAction()
    };
    // clang-format on

    return validAvTransportActions;
}

ServiceControlPointDefinition validAvTranportServiceSCPD() noexcept
{
    // clang-format off
    static auto const validAvTranportServiceSCPD = ServiceControlPointDefinition
    {
        QStringLiteral("http://127.0.0.1/AVTransport.xml"),
        validAvTransportStateVariables(),
        validAvTranportActions(),
    };
    // clang-format on

    return validAvTranportServiceSCPD;
}

ServiceDescription validAvTransportServiceDescription() noexcept
{
    // clang-format off
    static auto const validAvTransportServiceDescription = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:AVTransport:1"),
        QStringLiteral("urn:upnp-org:serviceId:AVTransport"),
        QStringLiteral("http://127.0.0.1/AVTransport.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return validAvTransportServiceDescription;
}

ServiceDescription eventUrlMissingInAvTransportDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:AVTransport:1"),
        QStringLiteral("urn:upnp-org:serviceId:AVTransport"),
        QStringLiteral("http://127.0.0.1/AVTransport.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QString("")
    };
    // clang-format on

    return desc;
}

ServiceDescription controlUrlMissingInAvTransportDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:AVTransport:1"),
        QStringLiteral("urn:upnp-org:serviceId:AVTransport"),
        QStringLiteral("http://127.0.0.1/AVTransport.xml"),
        QString(""),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return desc;
}

ServiceDescription serviceUrlMissingInAvTransportDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:AVTransport:1"),
        QString(""),
        QString("http://127.0.0.1/AVTransport.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return desc;
}

ServiceDescription scpdUrlMissingInAvTransportDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:AVTransport:1"),
        QStringLiteral("urn:upnp-org:serviceId:AVTransport"),
        QString(""),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return desc;
}
} // namespace UPnPAV