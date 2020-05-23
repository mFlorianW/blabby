#include "MediaServerShould.h"
#include "MediaServer.h"
#include "InvalidDeviceDescription.h"
#include "Descriptions.h"
#include "DeviceDescription.h"
#include "ContentDirectoryStateVariables.h"
#include "ContentDirectoryActions.h"

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
    QVector<SCPDStateVariable> variables = validConnectionManagerStateVariables;

    variables.removeAll(variable);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ConnectionManager.xml",
        variables,
        validConnectionManagerActions
    };
}

ServiceControlPointDefinition MediaServerShould::createConnectionManagerSCPDWithoutAction(const SCPDAction &action)
{
    QVector<SCPDAction> actions = validConnectionManagerActions;
    actions.removeAll(action);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ConnectionManager.xml",
        validConnectionManagerStateVariables,
        actions
    };
}

ServiceControlPointDefinition
MediaServerShould::createContentDirectorySCPDWithoutStateVariable(const SCPDStateVariable &variable)
{
    QVector<SCPDStateVariable> variables = validContentDirectoryStateVariables;
    variables.removeAll(variable);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ContentDirectory.xml",
        variables,
        validContentDirectoryActions
    };
}

ServiceControlPointDefinition MediaServerShould::createContentDirectorySCPDWithoutAction(const SCPDAction &action)
{
    QVector<SCPDAction> actions = validContentDirectoryActions;
    actions.removeAll(action);

    return ServiceControlPointDefinition
    {
        "http://127.0.0.1/ContentDirectory.xml",
        validContentDirectoryStateVariables,
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validConnectionManagerSCPD,
            validContentDirectorySCPD
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
            validContentDirectorySCPD
        });

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD not found."));
    }
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription SourceProtocolInfo_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(SourceProtocolInfo), validContentDirectorySCPD}
    };

    QTest::newRow("State variable SourceProtocolInfo missing")
            << SourceProtocolInfo_Missing
            << "ConnectionManager.*SourceProtocolInfo";

    DeviceDescription SinkProtocolInfo_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(SinkProtocolInfo), validContentDirectorySCPD}
    };

    QTest::newRow("State variable SinkProtocolInfo missing")
            << SinkProtocolInfo_Missing
            << "ConnectionManager.*SinkProtocolInfo";

    DeviceDescription CurrentConnectionIDs_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(CurrentConnectionIDs), validContentDirectorySCPD}
    };

    QTest::newRow("State variable CurrentConnectionIDs missing")
            << CurrentConnectionIDs_Missing
            << "ConnectionManager.*CurrentConnectionIDs";

    DeviceDescription A_ARG_TYPE_ConnectionStatus_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionStatus), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_ConnectionStatus missing")
            << A_ARG_TYPE_ConnectionStatus_Missing
            << "ConnectionManager.*A_ARG_TYPE_ConnectionStatus";


    DeviceDescription A_ARG_TYPE_ConnectionManager_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionManager), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_ConnectionManager missing")
            << A_ARG_TYPE_ConnectionManager_Missing
            << "ConnectionManager.*A_ARG_TYPE_ConnectionManager";

    DeviceDescription A_ARG_TYPE_Direction_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_Direction), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_Direction missing")
            << A_ARG_TYPE_Direction_Missing
            << "ConnectionManager.*A_ARG_TYPE_Direction";

    DeviceDescription A_ARG_TYPE_ProtocolInfo_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ProtocolInfo), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_ProtocolInfo missing")
            << A_ARG_TYPE_ProtocolInfo_Missing
            << "ConnectionManager.*A_ARG_TYPE_ProtocolInfo";

    DeviceDescription A_ARG_TYPE_ConnectionID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionID), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_ConnectionID missing")
            << A_ARG_TYPE_ConnectionID_Missing
            << "ConnectionManager.*A_ARG_TYPE_ConnectionID";

    DeviceDescription A_ARG_TYPE_RcsID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_RcsID), validContentDirectorySCPD}
    };

    QTest::newRow("State variable A_ARG_TYPE_RcsID missing")
            << A_ARG_TYPE_RcsID_Missing
            << "ConnectionManager.*A_ARG_TYPE_RcsID";
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer{DeviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaServerShould::Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription GetProtocolInfo_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutAction(GetProtocolInfo), validContentDirectorySCPD}
    };

    QTest::newRow("Action GetProtocolInfo missing")
            << GetProtocolInfo_Missing
            << "ConnectionManager.*GetProtocolInfo";

    DeviceDescription GetCurrentConnectionIDs_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutAction(GetCurrentConnectionIDs), validContentDirectorySCPD}
    };

    QTest::newRow("Action GetCurrentConnectionIDs missing")
            << GetCurrentConnectionIDs_Missing
            << "ConnectionManager.*GetCurrentConnectionIDs";

    DeviceDescription GetCurrentConnectionInfo_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { createConnectionManagerSCPDWithoutAction(GetCurrentConnectionInfo), validContentDirectorySCPD}
    };

    QTest::newRow("Action GetCurrentConnectionInfo missing")
            << GetCurrentConnectionInfo_Missing
            << "ConnectionManager.*GetCurrentConnectionInfo";
}

void MediaServerShould::Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer{DeviceDescription};

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription A_ARG_TYPE_ObjectID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_ObjectID)}
    };

    QTest::newRow("State variable A_ARG_TYPE_ObjectID missing")
            << A_ARG_TYPE_ObjectID_Missing
            << "ContentDirectory.*A_ARG_TYPE_ObjectID";

    DeviceDescription A_ARG_TYPE_Result_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Result)}
    };

    QTest::newRow("State variable A_ARG_TYPE_Result missing")
            << A_ARG_TYPE_Result_Missing
            << "ContentDirectory.*A_ARG_TYPE_Result";

    DeviceDescription A_ARG_TYPE_BrowseFlag_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_BrowseFlag)}
    };

    QTest::newRow("State variable A_ARG_TYPE_BrowseFlag missing")
            << A_ARG_TYPE_BrowseFlag_Missing
            << "ContentDirectory.*A_ARG_TYPE_BrowseFlag";

    DeviceDescription A_ARG_TYPE_Filter_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Filter)}
    };

    QTest::newRow("State variable A_ARG_TYPE_Filter missing")
            << A_ARG_TYPE_Filter_Missing
            << "ContentDirectory.*A_ARG_TYPE_Filter";

    DeviceDescription A_ARG_TYPE_SortCriteria_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_SortCriteria)}
    };

    QTest::newRow("State variable A_ARG_TYPE_SortCriteria missing")
            << A_ARG_TYPE_SortCriteria_Missing
            << "ContentDirectory.*A_ARG_TYPE_SortCriteria";

    DeviceDescription A_ARG_TYPE_Index_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Index)}
    };

    QTest::newRow("State variable A_ARG_TYPE_Index missing")
            << A_ARG_TYPE_Index_Missing
            << "ContentDirectory.*A_ARG_TYPE_Index";

    DeviceDescription A_ARG_TYPE_Count_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Count)}
    };

    QTest::newRow("State variable A_ARG_TYPE_Count missing")
            << A_ARG_TYPE_Count_Missing
            << "ContentDirectory.*A_ARG_TYPE_Count";

    DeviceDescription A_ARG_TYPE_UpdateID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_UpdateID)}
    };

    QTest::newRow("State variable A_ARG_TYPE_Count missing")
            << A_ARG_TYPE_UpdateID_Missing
            << "ContentDirectory.*A_ARG_TYPE_UpdateID";

    DeviceDescription SearchCapabilities_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(SearchCapabilities)}
    };

    QTest::newRow("State variable SearchCapabilities missing")
            << SearchCapabilities_Missing
            << "ContentDirectory.*SearchCapabilities";

    DeviceDescription SortCapabilities_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(SortCapabilities)}
    };

    QTest::newRow("State variable SortCapabilities missing")
            << SortCapabilities_Missing
            << "ContentDirectory.*SortCapabilities";

    DeviceDescription UpdateID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutStateVariable(SystemUpdateID)}
    };

    QTest::newRow("State variable SystemUpdateID missing")
            << UpdateID_Missing
            << "ContentDirectory.*SystemUpdateID";
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer{DeviceDescription};

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaServerShould::throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription GetSearchCapabilities_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutAction(GetSearchCapabilities)}
    };

    QTest::newRow("Action GetSearchCapabilities missing")
            << GetSearchCapabilities_Missing
            << "ContentDirectory.*GetSearchCapabilities";

    DeviceDescription GetSortCapabilities_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutAction(GetSortCapabilities)}
    };

    QTest::newRow("Action GetSortCapabilities missing")
            << GetSortCapabilities_Missing
            << "ContentDirectory.*GetSortCapabilities";

    DeviceDescription GetSystemUpdateID_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutAction(GetSystemUpdateID)}
    };

    QTest::newRow("Action GetSystemUpdateID missing")
            << GetSystemUpdateID_Missing
            << "ContentDirectory.*GetSystemUpdateID";

    DeviceDescription Browse_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        { validContentDirectoryDescription, validConnectionManagerDescription},
        { validConnectionManagerSCPD, createContentDirectorySCPDWithoutAction(Browse)}
    };

    QTest::newRow("Action Browse missing")
            << Browse_Missing
            << "ContentDirectory.*Browse";
}

void MediaServerShould::throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer{DeviceDescription};

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerShould);
