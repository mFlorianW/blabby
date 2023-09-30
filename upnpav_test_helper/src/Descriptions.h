// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DESCRIPTIONS_H
#define DESCRIPTIONS_H

#include "ConnectionManagerActions.h"
#include "ConnectionManagerStateVariables.h"
#include "ContentDirectoryActions.h"
#include "ContentDirectoryStateVariables.h"
#include "ServiceControlPointDefinition.h"
#include "ServiceDescription.h"

// clang-format off
namespace UPnPAV
{

static const ServiceDescription validContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "http://127.0.0.1/ContentDirectory.xml",
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription eventUrlMissingInContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "http://127.0.0.1/ContentDirectory.xml",
    "http://127.0.0.1/test/controlUrl",
    ""
};

static const ServiceDescription controlUrlMissingInContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "http://127.0.0.1/ContentDirectory.xml",
    "",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription serviceIdMissingInContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "",
    "http://127.0.0.1/ContentDirectory.xml",
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription scpdUrlMissingInContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "",
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription validConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "http://127.0.0.1/ConnectionManager.xml",
    "http://127.0.0.1/connectionManager/controlUrl",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const ServiceDescription eventUrlMissingInConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "http://127.0.0.1/ConnectionManager.xml",
    "http://127.0.0.1/connectionManager/controlUrl",
    ""
};

static const ServiceDescription controlUrlMissingInConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "http://127.0.0.1/ConnectionManager.xml",
    "",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const ServiceDescription serviceIdMissingInConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "",
    "http://127.0.0.1/ConnectionManager.xml",
    "http://127.0.0.1/connectionManager/controlUrl",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const ServiceDescription scpdUrlMissingInConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "",
    "http://127.0.0.1/connectionManager/controlUrl",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static QVector<SCPDStateVariable> validConnectionManagerStateVariables
{
    SourceProtocolInfo,
    SinkProtocolInfo,
    CurrentConnectionIDs,
    A_ARG_TYPE_ConnectionStatus,
    A_ARG_TYPE_ConnectionManager,
    A_ARG_TYPE_Direction,
    A_ARG_TYPE_ProtocolInfo,
    A_ARG_TYPE_ConnectionID,
    A_ARG_TYPE_AVTransportID,
    A_ARG_TYPE_RcsID
};

static QVector<SCPDAction> validConnectionManagerActions
{
    GetProtocolInfo,
    GetCurrentConnectionIDs,
    GetCurrentConnectionInfo
};

static const ServiceControlPointDefinition validConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",
    validConnectionManagerStateVariables,
    validConnectionManagerActions
};

static QVector<SCPDStateVariable> validContentDirectoryStateVariables
{
    A_ARG_TYPE_ObjectID,
    A_ARG_TYPE_Result,
    A_ARG_TYPE_BrowseFlag,
    A_ARG_TYPE_Filter,
    A_ARG_TYPE_SortCriteria,
    A_ARG_TYPE_Index,
    A_ARG_TYPE_Count,
    A_ARG_TYPE_UpdateID,
    SearchCapabilities,
    SortCapabilities,
    SystemUpdateID
};

static QVector<SCPDAction> validContentDirectoryActions
{
    GetSearchCapabilities,
    GetSortCapabilities,
    GetSystemUpdateID,
    Browse
};

static ServiceControlPointDefinition validContentDirectorySCPD
{
    "http://127.0.0.1/ContentDirectory.xml",
    validContentDirectoryStateVariables,
    validContentDirectoryActions
};

ServiceControlPointDefinition validAvTranportServiceSCPD() noexcept;

ServiceDescription validAvTransportServiceDescription() noexcept;

ServiceDescription eventUrlMissingInAvTransportDescription() noexcept;

ServiceDescription serviceUrlMissingInAvTransportDescription() noexcept;

ServiceDescription scpdUrlMissingInAvTransportDescription() noexcept;

ServiceDescription controlUrlMissingInAvTransportDescription() noexcept;


} //namespace UPnPAV

//clang-format on

#endif // DESCRIPTIONS_H
