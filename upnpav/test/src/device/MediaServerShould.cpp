#include "MediaServerShould.h"
#include "MediaServer.h"
#include "InvalidDeviceDescription.h"
#include "Descriptions.h"

#include <QTest>

namespace UPnPAV
{

MediaServerShould::MediaServerShould()
    : QObject()
{
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_ContentDirectory_Service_Description()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory description not found"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                eventUrlMissingInContentDirectoryDescription,
                validConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory event URL"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_Control_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                controlUrlMissingInContentDirectoryDescription,
                validConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory control URL"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_ServiceId()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                serviceIdMissingInContentDirectoryDescription,
                validConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory service ID"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_SCPD_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                scpdUrlMissingInContentDirectoryDescription,
                validConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory SCPD URL"));
    }
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validContentDirectoryDescription,
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager description not found"));
    }
}

void MediaServerShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validContentDirectoryDescription,
                eventUrlMissingInConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager event URL is not set"));
    }
}

void MediaServerShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validContentDirectoryDescription,
                controlUrlMissingInConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager control URL is not set"));
    }
}

void MediaServerShould::throw_An_Exception_When_ConnectionManager_Description_Has_ServiceId()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validContentDirectoryDescription,
                serviceIdMissingInConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager service ID is not set"));
    }
}

void MediaServerShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url()
{
    try
    {
        DeviceDescription deviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            QVector<ServiceDescription>
            {
                validContentDirectoryDescription,
                scpdUrlMissingInConnectionManagerDescription
            }
        };

        MediaServer mediaServer{deviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD URL is not set"));
    }
}

} // namespace UPnPAV
