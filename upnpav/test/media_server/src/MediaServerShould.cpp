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

ServiceControlPointDefinition
MediaServerShould::createConnectionManagerSCPDWithoutStateVariable(const SCPDStateVariable &variable)
{
    QVector<SCPDStateVariable> variables
    //StateVariables
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

    variables.removeAll(variable);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ConnectionManager.xml",
        variables,
        {
            GetProtocolInfo,
            GetCurrentConnectionIDs,
            GetCurrentConnectionInfo
        }
    };
}

ServiceControlPointDefinition MediaServerShould::createConnectionManagerSCPDWithoutAction(const SCPDAction &action)
{
    QVector<SCPDAction> actions
    {
        GetProtocolInfo,
        GetCurrentConnectionIDs,
        GetCurrentConnectionInfo
    };

    actions.removeAll(action);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ConnectionManager.xml",
        //StateVariables
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
        },
        actions
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
            createConnectionManagerSCPDWithoutStateVariable(SourceProtocolInfo)
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
            createConnectionManagerSCPDWithoutStateVariable(SinkProtocolInfo)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*SinkProtocolInfo")));
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
            createConnectionManagerSCPDWithoutStateVariable(CurrentConnectionIDs)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*CurrentConnectionConnectionIDs")));
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
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionStatus)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_ConnectionStatus")));
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
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionManager)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_ConnectionManager")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_Direction_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_Direction)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_Direction")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_ProtocolInfo_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ProtocolInfo)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_ProtocolInfo")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_ConnectionID_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionID)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_ConnectionID")));
    }
}

void MediaServerShould::throw_Exception_When_StateVar_A_ARG_TYPE_RcsID_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_RcsID)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*A_ARG_TYPE_RcsID")));
    }
}

void MediaServerShould::throw_Exception_When_Action_GetProtocolInfo_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutAction(GetProtocolInfo)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*GetProtocolInfo")));
    }
}

void MediaServerShould::throw_Exception_When_Action_GetCurrentConnectionIDs_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutAction(GetCurrentConnectionIDs)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*GetCurrentConnectionIDs")));
    }
}

void MediaServerShould::throw_Exception_When_Action_GetCurrentConnectionInfo_Misses_in_ConnectionManager_SCPD()
{
    try
    {
        MediaServer mediaServer = createMediaServer(
        {
            validContentDirectoryDescription,
            validConnectionManagerDescription,
        },
        {
            createConnectionManagerSCPDWithoutAction(GetCurrentConnectionInfo)
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains(QRegExp("ConnectionManager.*GetCurrentConnectionInfo")));
    }
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerShould);
