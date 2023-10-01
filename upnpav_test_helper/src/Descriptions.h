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
    QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
    QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
    QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
    QStringLiteral("http://127.0.0.1/test/controlUrl"),
    QStringLiteral("http://127.0.0.1/test/eventUrl")
};

static const ServiceDescription eventUrlMissingInContentDirectoryDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
    QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
    QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
    QStringLiteral("http://127.0.0.1/test/controlUrl"),
    QString("")
};

static const ServiceDescription controlUrlMissingInContentDirectoryDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
    QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
    QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
    QString(""),
    QStringLiteral("http://127.0.0.1/test/eventUrl")
};

static const ServiceDescription serviceIdMissingInContentDirectoryDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
    QString(""),
    QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
    QStringLiteral("http://127.0.0.1/test/controlUrl"),
    QStringLiteral("http://127.0.0.1/test/eventUrl")
};

static const ServiceDescription scpdUrlMissingInContentDirectoryDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:1"),
    QStringLiteral("urn:upnp-org:serviceId:ContentDirectory"),
    QString(""),
    QStringLiteral("http://127.0.0.1/test/controlUrl"),
    QStringLiteral("http://127.0.0.1/test/eventUrl")
};

static const ServiceDescription validConnectionManagerDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
    QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
    QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
    QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
    QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")
};

static const ServiceDescription eventUrlMissingInConnectionManagerDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
    QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
    QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
    QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
    QString("")
};

static const ServiceDescription controlUrlMissingInConnectionManagerDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
    QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
    QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
    QString(""),
    QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")
};

static const ServiceDescription serviceIdMissingInConnectionManagerDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
    QString(""),
    QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
    QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
    QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")
};

static const ServiceDescription scpdUrlMissingInConnectionManagerDescription
{
    QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager:1"),
    QStringLiteral("urn:upnp-org:serviceId:ConnectionManager"),
    QString(""),
    QStringLiteral("http://127.0.0.1/connectionManager/controlUrl"),
    QStringLiteral("http://127.0.0.1/connectionManager/eventUrl")
};

static QVector<SCPDStateVariable> const validConnectionManagerStateVariables
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

static QVector<SCPDAction> const validConnectionManagerActions
{
    GetProtocolInfo,
    GetCurrentConnectionIDs,
    GetCurrentConnectionInfo
};

static const ServiceControlPointDefinition validConnectionManagerSCPD
{
    QStringLiteral("http://127.0.0.1/ConnectionManager.xml"),
    validConnectionManagerStateVariables,
    validConnectionManagerActions
};

static QVector<SCPDStateVariable> const validContentDirectoryStateVariables
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

static QVector<SCPDAction> const validContentDirectoryActions
{
    GetSearchCapabilities,
    GetSortCapabilities,
    GetSystemUpdateID,
    Browse
};

static ServiceControlPointDefinition const validContentDirectorySCPD
{
    QStringLiteral("http://127.0.0.1/ContentDirectory.xml"),
    validContentDirectoryStateVariables,
    validContentDirectoryActions
};

QVector<SCPDStateVariable> validAvTransportStateVariables() noexcept;

QVector<SCPDAction> validAvTranportActions() noexcept;

ServiceControlPointDefinition validAvTranportServiceSCPD() noexcept;

ServiceDescription validAvTransportServiceDescription() noexcept;

ServiceDescription eventUrlMissingInAvTransportDescription() noexcept;

ServiceDescription serviceUrlMissingInAvTransportDescription() noexcept;

ServiceDescription scpdUrlMissingInAvTransportDescription() noexcept;

ServiceDescription controlUrlMissingInAvTransportDescription() noexcept;


} //namespace UPnPAV

//clang-format on

#endif // DESCRIPTIONS_H
