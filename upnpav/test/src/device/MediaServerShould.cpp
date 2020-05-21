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

MediaServer MediaServerShould::createMediaServer(const QVector<ServiceDescription> &services)
{
    return MediaServer
    {
        DeviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            services
         }
    };
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_ContentDirectory_Service_Description()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            eventUrlMissingInContentDirectoryDescription,
            validConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            controlUrlMissingInContentDirectoryDescription,
            validConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            serviceIdMissingInContentDirectoryDescription,
            validConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            scpdUrlMissingInContentDirectoryDescription,
            validConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            eventUrlMissingInConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            controlUrlMissingInConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            serviceIdMissingInConnectionManagerDescription
        });

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
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            scpdUrlMissingInConnectionManagerDescription
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD URL is not set"));
    }
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ContentDirectory()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory SCPD not found."));
    }
}

} // namespace UPnPAV
