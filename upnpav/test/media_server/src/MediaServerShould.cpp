#include "MediaServerShould.h"
#include "MediaServer.h"
#include "InvalidDeviceDescription.h"
#include "Descriptions.h"
#include "DeviceDescription.h"

#include <QTest>
#include <QDebug>

namespace UPnPAV
{

MediaServerShould::MediaServerShould()
    : QObject()
{
}

MediaServer MediaServerShould::createMediaServer(const QVector<ServiceDescription> &services,
                                                 const QVector<ServiceControlPointDefinition> &scpds)
{
    return MediaServer
    {
        DeviceDescription
        {
            "", "", "", "", "",
            QVector<IconDescription>{},

            services,
            scpds
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
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
        },
        {
            validConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch(InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD URL is not set"));
    }
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription
        },
        {
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD not found."));
    }
}

void MediaServerShould::throw_An_Exception_When_StateVariable_SourceProtocolInfo_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            sourceProtocolInfoMissingInConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*SourceProtocolInfo")));
    }
}

void MediaServerShould::throw_An_Exception_When_StateVariable_SinkProtocolInfo_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            sinkProtocolInfoMissingInConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager|SinkProtocolInfo")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_CurrentConnectionConnectionIDs_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            currentConnectionConnectionIDsMissingInConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager|CurrentConnectionConnectionIDs")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_ConnectionStatus_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            a_ARG_TYPE_ConnectionStatusMissingInConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager|CurrentConnectionConnectionIDs")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_ConnectionManager_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            a_ARG_TYPE_ConnectionManagerMissingInConnectionManagerSCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager|A_ARG_TYPE_ConnectionManager")));
    }
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerShould);
#include <MediaServerShould.moc>
