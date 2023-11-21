// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerShould.hpp"
#include "ContentDirectoryActions.hpp"
#include "ContentDirectoryStateVariables.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaServer.hpp"
#include "SoapMessageTransmitterDouble.hpp"
#include <QDebug>
#include <QTest>

namespace UPnPAV
{

MediaServerShould::MediaServerShould()
    : QObject{}
{
}

MediaServerShould::~MediaServerShould() = default;

MediaServer MediaServerShould::createMediaServer(DeviceDescription &deviceDescription)
{
    return MediaServer{deviceDescription, m_soapMessageTransmitter};
}

void MediaServerShould::init()
{
    m_soapMessageTransmitter = QSharedPointer<SoapMessageTransmitterDouble>{new SoapMessageTransmitterDouble()};
}

MediaServer MediaServerShould::createMediaServer(const QVector<ServiceDescription> &services,
                                                 const QVector<ServiceControlPointDefinition> &scpds)

{
    IconDescription iconDes{"", 0, 0, 24, "http://localhost:8200/icons/sm.png"};
    return MediaServer{
        DeviceDescription{"", "MediaServerName", "", "", "", QVector<IconDescription>{iconDes}, services, scpds},
        m_soapMessageTransmitter};
}

ServiceControlPointDefinition MediaServerShould::createContentDirectorySCPDWithoutStateVariable(
    const SCPDStateVariable &variable)
{
    QVector<SCPDStateVariable> variables = validContentDirectoryStateVariables();
    variables.removeAll(variable);

    return ServiceControlPointDefinition{"http://127.0.0.1/ContentDirectory.xml",
                                         variables,
                                         validContentDirectoryActions()};
}

ServiceControlPointDefinition MediaServerShould::createContentDirectorySCPDWithoutAction(const SCPDAction &action)
{
    QVector<SCPDAction> actions = validContentDirectoryActions();
    actions.removeAll(action);

    return ServiceControlPointDefinition{"http://127.0.0.1/ContentDirectory.xml",
                                         validContentDirectoryStateVariables(),
                                         actions};
}

void MediaServerShould::throw_An_Exception_When_DeviceDescription_Has_No_ContentDirectory_Service_Description()
{
    try
    {
        MediaServer mediaServer = createMediaServer({validConnectionManagerDescription()},
                                                    {validConnectionManagerSCPD(), validContentDirectorySCPD()});

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory description not found"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_Url()
{
    try
    {
        MediaServer mediaServer =
            createMediaServer({eventUrlMissingInContentDirectoryDescription(), validConnectionManagerDescription()},
                              {validConnectionManagerSCPD(), validContentDirectorySCPD()});

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory event URL"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_Control_Url()
{
    try
    {
        MediaServer mediaServer =
            createMediaServer({controlUrlMissingInContentDirectoryDescription(), validConnectionManagerDescription()},
                              {validConnectionManagerSCPD(), validContentDirectorySCPD()});

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory control URL"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_ServiceId()
{
    try
    {
        MediaServer mediaServer =
            createMediaServer({serviceIdMissingInContentDirectoryDescription(), validConnectionManagerDescription()},
                              {validConnectionManagerSCPD(), validContentDirectorySCPD()});

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory service ID"));
    }
}

void MediaServerShould::throw_An_Exception_When_ContentDirectory_Description_Has_No_SCPD_Url()
{
    try
    {
        MediaServer mediaServer =
            createMediaServer({scpdUrlMissingInContentDirectoryDescription(), validConnectionManagerDescription()},
                              {validConnectionManagerSCPD(), validContentDirectorySCPD()});

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ContentDirectory SCPD URL"));
    }
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription A_ARG_TYPE_ObjectID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_ObjectID())}};

    QTest::newRow("State variable A_ARG_TYPE_ObjectID missing")
        << A_ARG_TYPE_ObjectID_Missing << "ContentDirectory.*A_ARG_TYPE_ObjectID";

    DeviceDescription A_ARG_TYPE_Result_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Result())}};

    QTest::newRow("State variable A_ARG_TYPE_Result missing")
        << A_ARG_TYPE_Result_Missing << "ContentDirectory.*A_ARG_TYPE_Result";

    DeviceDescription A_ARG_TYPE_BrowseFlag_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_BrowseFlag())}};

    QTest::newRow("State variable A_ARG_TYPE_BrowseFlag missing")
        << A_ARG_TYPE_BrowseFlag_Missing << "ContentDirectory.*A_ARG_TYPE_BrowseFlag";

    DeviceDescription A_ARG_TYPE_Filter_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Filter())}};

    QTest::newRow("State variable A_ARG_TYPE_Filter missing")
        << A_ARG_TYPE_Filter_Missing << "ContentDirectory.*A_ARG_TYPE_Filter";

    DeviceDescription A_ARG_TYPE_SortCriteria_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_SortCriteria())}};

    QTest::newRow("State variable A_ARG_TYPE_SortCriteria missing")
        << A_ARG_TYPE_SortCriteria_Missing << "ContentDirectory.*A_ARG_TYPE_SortCriteria";

    DeviceDescription A_ARG_TYPE_Index_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Index())}};

    QTest::newRow("State variable A_ARG_TYPE_Index missing")
        << A_ARG_TYPE_Index_Missing << "ContentDirectory.*A_ARG_TYPE_Index";

    DeviceDescription A_ARG_TYPE_Count_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_Count())}};

    QTest::newRow("State variable A_ARG_TYPE_Count missing")
        << A_ARG_TYPE_Count_Missing << "ContentDirectory.*A_ARG_TYPE_Count";

    DeviceDescription A_ARG_TYPE_UpdateID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(A_ARG_TYPE_UpdateID())}};

    QTest::newRow("State variable A_ARG_TYPE_Count missing")
        << A_ARG_TYPE_UpdateID_Missing << "ContentDirectory.*A_ARG_TYPE_UpdateID";

    DeviceDescription SearchCapabilities_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(SearchCapabilities())}};

    QTest::newRow("State variable SearchCapabilities missing")
        << SearchCapabilities_Missing << "ContentDirectory.*SearchCapabilities";

    DeviceDescription SortCapabilities_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(SortCapabilities())}};

    QTest::newRow("State variable SortCapabilities missing")
        << SortCapabilities_Missing << "ContentDirectory.*SortCapabilities";

    DeviceDescription UpdateID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutStateVariable(SystemUpdateID())}};

    QTest::newRow("State variable SystemUpdateID missing") << UpdateID_Missing << "ContentDirectory.*SystemUpdateID";
}

void MediaServerShould::throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer = createMediaServer(DeviceDescription);

        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaServerShould::throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription GetSearchCapabilities_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutAction(GetSearchCapabilities())}};

    QTest::newRow("Action GetSearchCapabilities missing")
        << GetSearchCapabilities_Missing << "ContentDirectory.*GetSearchCapabilities";

    DeviceDescription GetSortCapabilities_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutAction(GetSortCapabilities())}};

    QTest::newRow("Action GetSortCapabilities missing")
        << GetSortCapabilities_Missing << "ContentDirectory.*GetSortCapabilities";

    DeviceDescription GetSystemUpdateID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validContentDirectoryDescription(), validConnectionManagerDescription()},
        {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutAction(GetSystemUpdateID())}};

    QTest::newRow("Action GetSystemUpdateID missing")
        << GetSystemUpdateID_Missing << "ContentDirectory.*GetSystemUpdateID";

    DeviceDescription Browse_Missing{"",
                                     "",
                                     "",
                                     "",
                                     "",
                                     QVector<IconDescription>{},
                                     {validContentDirectoryDescription(), validConnectionManagerDescription()},
                                     {validConnectionManagerSCPD(), createContentDirectorySCPDWithoutAction(Browse())}};

    QTest::newRow("Action Browse missing") << Browse_Missing << "ContentDirectory.*Browse";
}

void MediaServerShould::throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        MediaServer mediaServer = createMediaServer(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaServerShould::shall_Send_The_SOAP_Message_When_Calling_GetSortCapabilities()
{
    MediaServer mediaServer =
        createMediaServer({validContentDirectoryDescription(), validConnectionManagerDescription()},
                          {validContentDirectorySCPD(), validConnectionManagerSCPD()});

    QString expectedSoapMessage{"<?xml version=\"1.0\"?>"
                                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                "<s:Body>"
                                "<u:GetSortCapabilities xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"/>"
                                "</s:Body>"
                                "</s:Envelope>"};

    mediaServer.getSortCapabilities();

    QVERIFY2(expectedSoapMessage == m_soapMessageTransmitter->xmlMessageBody(),
             QString{"Expected: %1 Actual: %2"}
                 .arg(expectedSoapMessage, m_soapMessageTransmitter->xmlMessageBody())
                 .toLocal8Bit());
}

void MediaServerShould::shall_Send_The_SOAP_Message_When_Calling_Browse()
{
    MediaServer mediaServer =
        createMediaServer({validContentDirectoryDescription(), validConnectionManagerDescription()},
                          {validContentDirectorySCPD(), validConnectionManagerSCPD()});

    // Important the order of the args must have the order of the SCPD description.
    QString expectedSoapMessage{"<?xml version=\"1.0\"?>"
                                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                "<s:Body>"
                                "<u:Browse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\">"
                                "<u:ObjectID>0</u:ObjectID>"
                                "<u:BrowseFlag>BrowseMetadata</u:BrowseFlag>"
                                "<u:Filter>*</u:Filter>"
                                "<u:StartingIndex>0</u:StartingIndex>"
                                "<u:RequestedCount>0</u:RequestedCount>"
                                "<u:SortCriteria></u:SortCriteria>"
                                "</u:Browse>"
                                "</s:Body>"
                                "</s:Envelope>"};

    mediaServer.browse("0", MediaServer::BrowseFlag::MetaData, "*", "");

    QVERIFY2(expectedSoapMessage == m_soapMessageTransmitter->xmlMessageBody(),
             QString{"Expected: %1 Actual: %2"}
                 .arg(expectedSoapMessage, m_soapMessageTransmitter->xmlMessageBody())
                 .toLocal8Bit());
}

void MediaServerShould::give_a_name()
{
    MediaServer mediaServer =
        createMediaServer({validContentDirectoryDescription(), validConnectionManagerDescription()},
                          {validContentDirectorySCPD(), validConnectionManagerSCPD()});
    const QString expectedMediaServerName{"MediaServerName"};

    auto mediaServerName = mediaServer.name();

    QCOMPARE(mediaServerName, expectedMediaServerName);
}

void MediaServerShould::give_a_icon_url()
{
    MediaServer mediaServer =
        createMediaServer({validContentDirectoryDescription(), validConnectionManagerDescription()},
                          {validContentDirectorySCPD(), validConnectionManagerSCPD()});
    const QUrl expectedMediaServerIconUrl{"http://localhost:8200/icons/sm.png"};

    auto mediaServerIconUrl = mediaServer.iconUrl();

    QCOMPARE(mediaServerIconUrl, expectedMediaServerIconUrl);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerShould);
