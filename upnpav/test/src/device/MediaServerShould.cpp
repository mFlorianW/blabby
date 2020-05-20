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
                eventUrlMissingContentDirectoryDescription,
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
                controlUrlMissingContentDirectoryDescription,
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
                serviceIdMissingContentDirectoryDescription,
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
                eventUrlMissingConnectionManagerDescription
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
                controlUrlMissingConnectionManagerDescription
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

//void MediaServerShould::be_Creatable_Only_With_ServiceDescription_ConnectionManager()
//{
//    try
//    {
//        MediaServer mediaServer{basicMediaServerDeviceDescription};
//    }
//    catch(InvalidDeviceDescription &e)
//    {
//        if(QString{e.what()} == "ConnectionManager description not found.")
//        {
//            QFAIL("The service description for the ConnectionManager is missing.");
//        }
//    }
//}

//void MediaServerShould::be_Creatable_Only_With_A_Non_Empty_EventUrl_In_ConnectionManager_Description()
//{
//    try
//    {
//        MediaServer mediaServer{basicMediaServerDeviceDescription};
//    }
//    catch (const InvalidDeviceDescription &e)
//    {
//        if(QString{e.what()} == "ConntectionManager event URL is not set.")
//        {
//            QFAIL("ConnectionManager event URL is not set.");
//        }
//    }
//}

//void MediaServerShould::be_Creatable_Only_With_A_Non_Empty_ControlUrl_In_ConnectionManager_Description()
//{
//    try
//    {
//        MediaServer mediaServer{basicMediaServerDeviceDescription};
//    }
//    catch (const InvalidDeviceDescription &e)
//    {
//        if(QString{e.what()} == "ConnectionManager control URL is not set.")
//        {
//            QFAIL("ConnectionManager control URL is not set.");
//        }
//    }
//}

//void MediaServerShould::be_Creatabe_Only_With_A_Non_Empty_ServiceId_In_ConnectionManager_Description()
//{
//    try
//    {
//        MediaServer mediaServer{basicMediaServerDeviceDescription};
//    }
//    catch (const InvalidDeviceDescription &e)
//    {
//        if(QString{e.what()} == "ConnectionManager service ID is not set.")
//        {
//            QFAIL("ConnectionManager service ID is not set.");
//        }
//    }
//}

} // namespace UPnPAV
