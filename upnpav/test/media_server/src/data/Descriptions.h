#ifndef DESCRIPTIONS_H
#define DESCRIPTIONS_H

#include "ServiceDescription.h"
#include "ServiceControlPointDefinition.h"
#include "ConnectionManagerStateVariables.h"
#include "ConnectionManagerActions.h"

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

static const ServiceControlPointDefinition validConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
        SourceProtocolInfo,
        SinkProtocolInfo,
        CurrentConnectionConnectionIDs,
        A_ARG_TYPE_ConnectionStatus,
        A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

static const ServiceControlPointDefinition sourceProtocolInfoMissingInConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
//        SourceProtocolInfo, //Remove this one for the test.
        SinkProtocolInfo,
        CurrentConnectionConnectionIDs,
        A_ARG_TYPE_ConnectionStatus,
        A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

static const ServiceControlPointDefinition sinkProtocolInfoMissingInConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
        SourceProtocolInfo,
        //SinkProtocolInfo, Remove this one for the test.
        CurrentConnectionConnectionIDs,
        A_ARG_TYPE_ConnectionStatus,
        A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

static const ServiceControlPointDefinition currentConnectionConnectionIDsMissingInConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
        SourceProtocolInfo,
        SinkProtocolInfo,
        //CurrentConnectionConnectionIDs,
        A_ARG_TYPE_ConnectionStatus,
        A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

static const ServiceControlPointDefinition a_ARG_TYPE_ConnectionStatusMissingInConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
        SourceProtocolInfo,
        SinkProtocolInfo,
        CurrentConnectionConnectionIDs,
        //A_ARG_TYPE_ConnectionStatus,
        A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

static const ServiceControlPointDefinition a_ARG_TYPE_ConnectionManagerMissingInConnectionManagerSCPD
{
    "http://127.0.0.1/ConnectionManager.xml",

    //StateVariables
    {
        SourceProtocolInfo,
        SinkProtocolInfo,
        CurrentConnectionConnectionIDs,
        A_ARG_TYPE_ConnectionStatus,
        //A_ARG_TYPE_ConnectionManager,
        A_ARG_TYPE_Direction,
        A_ARG_TYPE_ProtocolInfo,
        A_ARG_TYPE_ConnectionID,
        A_ARG_TYPE_AVTransportID,
        A_ARG_TYPE_RcsID
    },

    //ActionList
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    }
};

} //namespace UPnPAV

#endif // DESCRIPTIONS_H
