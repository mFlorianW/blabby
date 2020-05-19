#include "MediaServerShould.h"
#include "DeviceDescription.h"
#include "MediaServer.h"
#include "InvalidDeviceDescription.h"

#include <QTest>

namespace UPnPAV
{

namespace
{
static const ServiceDescription basicContentDirectoryDescription
{
    QString{"urn:schemas-upnp-org:service:ContentDirectory:1"},
    "urn:upnp-org:serviceId:ContentDirectory",
    "", //the scpd url is unneeded.
    "http://127.0.0.1/test/controlUrl",
    "http://127.0.0.1/test/eventUrl"
};

static const ServiceDescription basicConnectionManagerDescription
{
    QString{"urn:schemas-upnp-org:service:ConnectionManager:1"},
    "",
    "",
    "",
    ""
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
        basicContentDirectoryDescription,
        basicConnectionManagerDescription
    }
};
}

MediaServerShould::MediaServerShould()
    : QObject()
{
}

void MediaServerShould::be_Creatable_Only_With_ServiceDescription_ContentDirectory()
{
    try
    {
        MediaServer mediaServer{basicMediaServerDeviceDescription};
    }
    catch(InvalidDeviceDescription &e)
    {
        if(QString{e.what()} == "ContentDirectory description not found.")
        {
            QFAIL("The service description for the ContentDirectory is missing.");
        }

        QFAIL("Passed invalid device description.");
    }
}

void MediaServerShould::be_Creatable_Only_With_ServiceDescription_ConnectionManager()
{
    try
    {
        MediaServer mediaServer{basicMediaServerDeviceDescription};
    }
    catch(InvalidDeviceDescription &e)
    {
        if(QString{e.what()} == "ConnectionManager description not found.")
        {
            QFAIL("The service description for the ConnectionManager is missing.");
        }

        QFAIL("Passed invalid device description.");
    }
}

void MediaServerShould::be_Creatable_Only_With_A_Non_Empty_EventUrl_In_ContentDirectory_Description()
{
    try
    {
        MediaServer mediaServer{basicMediaServerDeviceDescription};
    }
    catch (const InvalidDeviceDescription &e)
    {
        if(QString{e.what()} == "ContentDirectory event URL is not set")
        {
            QFAIL("ContentDirectory event url is not set");
        }

        QFAIL("Passed invalid device description.");
    }
}

void MediaServerShould::be_Creatable_Only_With_A_Non_Empty_ContrlUrl_In_ContentDirectory_Description()
{
    try
    {
        MediaServer mediaServer{basicMediaServerDeviceDescription};
    }
    catch (const InvalidDeviceDescription &e)
    {
        if(QString{e.what()} == "ContentDirectory control URL is not set.")
        {
            QFAIL("ContentDirectory control url is not set");
        }

        QFAIL("Passed invalid device description.");
    }
}

void MediaServerShould::be_Creatable_Only_With_A_Non_Empty_ServiceId_In_ContentDirectory_Description()
{
    try
    {
        MediaServer mediaServer{basicMediaServerDeviceDescription};
    }
    catch (const InvalidDeviceDescription &e)
    {
        if(QString{e.what()} == "ContentDirectory service ID is not set.")
        {
            QFAIL("ContentDirectory service ID is not set.");
        }

        QFAIL("Passed invalid device description.");
    }
}

} // namespace UPnPAV
