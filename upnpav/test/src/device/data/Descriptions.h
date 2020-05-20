#ifndef DESCRIPTIONS_H
#define DESCRIPTIONS_H

#include "DeviceDescription.h"

namespace UPnPAV
{

static const ServiceDescription validContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "", //the scpd url is unneeded.
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription eventUrlMissingContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "", //the scpd url is unneeded.
    "http://127.0.0.1/test/controlUrl",
    ""
};

static const ServiceDescription controlUrlMissingContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "", //the scpd url is unneeded.
    "",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription serviceIdMissingContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "",
    "", //the scpd url is unneeded.
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription validConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "",
    "http://127.0.0.1/connectionManager/controlUrl",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const ServiceDescription eventUrlMissingConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "",
    "http://127.0.0.1/connectionManager/controlUrl",
    ""
};

static const ServiceDescription controlUrlMissingConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "urn:upnp-org:serviceId:ConnectionManager",
    "",
    "",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const ServiceDescription serviceIdMissingInConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "",
    "",
    "http://127.0.0.1/connectionManager/controlUrl",
    "http://127.0.0.1/connectionManager/eventUrl"
};

static const DeviceDescription basicMediaServerDeviceDescription
{
    "",
    "",
    "",
    "",
    "",
    QVector<IconDescription>
    {
    },

    QVector<ServiceDescription>
    {
        validContentDirectoryDescription,
        validConnectionManagerDescription
    }
};

} //namespace UPnPAV

#endif // DESCRIPTIONS_H
