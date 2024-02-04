// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Descriptions.hpp"
#include "AvTransportActions.hpp"
#include "AvTransportStateVariables.hpp"
#include "ConnectionManagerActions.hpp"
#include "ConnectionManagerStateVariables.hpp"
#include "ContentDirectoryActions.hpp"
#include "ContentDirectoryStateVariables.hpp"
#include "RenderingControlActions.hpp"
#include "RenderingControlStateVariables.hpp"

namespace UPnPAV
{

ServiceDescription validContentDirectoryDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
                                          QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
                                          QStringLiteral("http://127.0.0.1/test/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/test/eventUrl")};
    return desc;
}

ServiceDescription eventUrlMissingInContentDirectoryDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
                                          QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
                                          QStringLiteral("http://127.0.0.1/test/controlUrl"),
                                          QString("")};
    return desc;
}

ServiceDescription controlUrlMissingInContentDirectoryDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
                                          QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/test/eventUrl")};
    return desc;
}

ServiceDescription serviceIdMissingInContentDirectoryDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
                                          QStringLiteral("http://127.0.0.1/test/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/test/eventUrl")};
    return desc;
}

ServiceDescription scpdUrlMissingInContentDirectoryDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/test/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/test/eventUrl")};
    return desc;
}

ServiceDescription validConnectionManagerDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
                                          QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")};
    return desc;
}

ServiceDescription eventUrlMissingInConnectionManagerDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
                                          QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
                                          QString("")};
    return desc;
}

ServiceDescription controlUrlMissingInConnectionManagerDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
                                          QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")};
    return desc;
}

ServiceDescription serviceIdMissingInConnectionManagerDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")};
    return desc;
}

ServiceDescription scpdUrlMissingInConnectionManagerDescription()
{
    static auto desc = ServiceDescription{QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
                                          QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
                                          QString(""),
                                          QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
                                          QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")};
    return desc;
}

QVector<SCPDStateVariable> validConnectionManagerStateVariables()
{
    static auto variables = QVector<SCPDStateVariable>{SourceProtocolInfo(),
                                                       SinkProtocolInfo(),
                                                       CurrentConnectionIDs(),
                                                       A_ARG_TYPE_ConnectionStatus(),
                                                       A_ARG_TYPE_ConnectionManager(),
                                                       A_ARG_TYPE_Direction(),
                                                       A_ARG_TYPE_ProtocolInfo(),
                                                       A_ARG_TYPE_ConnectionID(),
                                                       A_ARG_TYPE_AVTransportID(),
                                                       A_ARG_TYPE_RcsID()};
    return variables;
}

QVector<SCPDAction> validConnectionManagerActions()
{
    static auto actions = QVector<SCPDAction>{GetProtocolInfo(), GetCurrentConnectionIDs(), GetCurrentConnectionInfo()};
    return actions;
}

ServiceControlPointDefinition validConnectionManagerSCPD()
{
    static auto scpd = ServiceControlPointDefinition{QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
                                                     validConnectionManagerStateVariables(),
                                                     validConnectionManagerActions()};
    return scpd;
}

QVector<SCPDStateVariable> validContentDirectoryStateVariables()
{
    static auto variables = QVector<SCPDStateVariable>{A_ARG_TYPE_ObjectID(),
                                                       A_ARG_TYPE_Result(),
                                                       A_ARG_TYPE_BrowseFlag(),
                                                       A_ARG_TYPE_Filter(),
                                                       A_ARG_TYPE_SortCriteria(),
                                                       A_ARG_TYPE_Index(),
                                                       A_ARG_TYPE_Count(),
                                                       A_ARG_TYPE_UpdateID(),
                                                       SearchCapabilities(),
                                                       SortCapabilities(),
                                                       SystemUpdateID()};
    return variables;
}

QVector<SCPDAction> validContentDirectoryActions()
{
    static auto actions =
        QVector<SCPDAction>{GetSearchCapabilities(), GetSortCapabilities(), GetSystemUpdateID(), Browse()};
    return actions;
}

ServiceControlPointDefinition validContentDirectorySCPD()
{
    static auto desc = ServiceControlPointDefinition{QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
                                                     validContentDirectoryStateVariables(),
                                                     validContentDirectoryActions()};
    return desc;
}

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
        createPreviousAction(),
        createPauseAction()
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
        QStringLiteral("http://127.0.0.1:27016/test/eventUrl")
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

QVector<SCPDStateVariable> validRenderingControlStateVariables() noexcept
{
    static auto vars = QVector<SCPDStateVariable>{
        lastChangeVariable(),
        presetNameListVariable(),
        a_ARG_TYPE_ChannelVariable(),
        a_ARG_TYPE_InstanceIDVariable(),
        a_ARG_TYPE_PresetNameVariable(),
    };
    return vars;
}

QVector<SCPDAction> validRenderingControlActions() noexcept
{
    static auto actions = QVector<SCPDAction>{
        listPresetsAction(),
        selectPresetAction(),
        getVolumeAction(),
    };
    return actions;
}

ServiceControlPointDefinition validRenderingControlSCPD() noexcept
{
    static auto const scpd = ServiceControlPointDefinition{
        QStringLiteral("http://127.0.0.1/RenderingControl.xml"),
        validRenderingControlStateVariables(),
        validRenderingControlActions(),
    };
    return scpd;
}

ServiceDescription validRenderingControlServiceDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:RenderingControl:1"),
        QStringLiteral("urn:upnp-org:serviceId:RenderingControl"),
        QStringLiteral("http://127.0.0.1/RenderingControl.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return desc;
}

ServiceDescription scpdUrlMissingInRenderingControlDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:RenderingControl:1"),
        QStringLiteral("urn:upnp-org:serviceId:RenderingControl"),
        QStringLiteral(""),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on
    return desc;
}

ServiceDescription eventUrlMissingInRenderingControlDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:RenderingControl:1"),
        QStringLiteral("urn:upnp-org:serviceId:RenderingControl"),
        QStringLiteral("http://127.0.0.1/RenderingControl.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("")
    };
    // clang-format on
    return desc;
}

ServiceDescription controlUrlMissingInRenderingControlDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:RenderingControl:1"),
        QStringLiteral("urn:upnp-org:serviceId:RenderingControl"),
        QStringLiteral("http://127.0.0.1/RenderingControl.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("")
    };
    // clang-format on
    return desc;
}

ServiceDescription serviceUrlMissingInRenderingControlDescription() noexcept
{
    // clang-format off
    static auto const desc = ServiceDescription
    {
        QStringLiteral("urn:schemas-upnp-org:service:RenderingControl:1"),
        QStringLiteral(""),
        QStringLiteral("http://127.0.0.1/RenderingControl.xml"),
        QStringLiteral("http://127.0.0.1/test/controlUrl"),
        QStringLiteral("http://127.0.0.1/test/eventUrl")
    };
    // clang-format on

    return desc;
}

DeviceDescription validRendererDeviceDescription() noexcept
{
    return DeviceDescription{QString(""),
                             QStringLiteral("MediaRenderer"),
                             QString(""),
                             QString(""),
                             QString(""),
                             {IconDescription{"", 0, 0, 24, "http://localhost:8200/icons/sm.png"}},
                             {validRenderingControlServiceDescription(), validConnectionManagerDescription()},
                             {validRenderingControlSCPD(), validConnectionManagerSCPD()}};
}

} // namespace UPnPAV
