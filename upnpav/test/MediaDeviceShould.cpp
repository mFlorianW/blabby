// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaDeviceShould.h"
#include "Descriptions.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "MediaDevice.h"
#include "SCPDAction.h"
#include "SCPDStateVariable.h"
#include "SoapMessageTransmitterDouble.h"
#include <QTest>

namespace UPnPAV
{

class TestMediaDevice : public MediaDevice
{
public:
    TestMediaDevice(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitterDouble> msgTransmitter)
        : MediaDevice{deviceDescription, msgTransmitter}
        , mMsgTransmitter{msgTransmitter}
    {
    }

protected:
    QSharedPointer<SoapMessageTransmitterDouble> mMsgTransmitter;
};

class MediaDeviceWithoutAV : public TestMediaDevice
{
public:
    MediaDeviceWithoutAV()
        : TestMediaDevice{
              DeviceDescription{"",
                                "MediaServerName",
                                "",
                                "",
                                "",
                                QVector<IconDescription>{{"", 0, 0, 24, "http://localhost:8200/icons/sm.png"}},
                                {validContentDirectoryDescription, validConnectionManagerDescription},
                                {validContentDirectorySCPD, validConnectionManagerSCPD}},
              QSharedPointer<SoapMessageTransmitterDouble>(new SoapMessageTransmitterDouble{})}
    {
    }

    MediaDeviceWithoutAV(DeviceDescription devDesc)
        : TestMediaDevice{devDesc, QSharedPointer<SoapMessageTransmitterDouble>(new SoapMessageTransmitterDouble{})}
    {
    }

    QString lastSoapCall() const noexcept
    {
        return mMsgTransmitter->xmlMessageBody();
    }
};

MediaDeviceShould::MediaDeviceShould()
    : QObject()
{
}

MediaDeviceShould::~MediaDeviceShould() = default;

ServiceControlPointDefinition MediaDeviceShould::createConnectionManagerSCPDWithoutStateVariable(
    const SCPDStateVariable &variable)
{
    QVector<SCPDStateVariable> variables = validConnectionManagerStateVariables;
    variables.removeAll(variable);

    return ServiceControlPointDefinition{"http://127.0.0.1/ConnectionManager.xml",
                                         variables,
                                         validConnectionManagerActions};
}

ServiceControlPointDefinition MediaDeviceShould::createConnectionManagerSCPDWithoutAction(const SCPDAction &action)
{
    QVector<SCPDAction> actions = validConnectionManagerActions;
    actions.removeAll(action);

    return ServiceControlPointDefinition{"http://127.0.0.1/ConnectionManager.xml",
                                         validConnectionManagerStateVariables,
                                         actions};
}

void MediaDeviceShould::throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription()
{
    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"", "", "", "", "", {}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager description not found"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url()
{
    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {eventUrlMissingInConnectionManagerDescription},
                                                                {validConnectionManagerSCPD}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager event URL is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_ServiceId()
{
    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {serviceIdMissingInConnectionManagerDescription},
                                                                {validConnectionManagerSCPD}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager service ID is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url()
{
    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {scpdUrlMissingInConnectionManagerDescription},
                                                                {validConnectionManagerSCPD}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD URL is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager()
{
    try
    {
        auto const dev =
            MediaDeviceWithoutAV(DeviceDescription{"", "", "", "", "", {}, {validConnectionManagerDescription}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD not found."));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url()
{
    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {controlUrlMissingInConnectionManagerDescription},
                                                                {validConnectionManagerSCPD}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (InvalidDeviceDescription &e)
    {
        QVERIFY(QString{e.what()}.contains("ConnectionManager control URL is not set"));
    }
}

void MediaDeviceShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription SourceProtocolInfo_Missing{"",
                                                 "",
                                                 "",
                                                 "",
                                                 "",
                                                 QVector<IconDescription>{},
                                                 {validConnectionManagerDescription},
                                                 {createConnectionManagerSCPDWithoutStateVariable(SourceProtocolInfo)}};

    QTest::newRow("State variable SourceProtocolInfo missing")
        << SourceProtocolInfo_Missing << "ConnectionManager.*SourceProtocolInfo";

    DeviceDescription SinkProtocolInfo_Missing{"",
                                               "",
                                               "",
                                               "",
                                               "",
                                               QVector<IconDescription>{},
                                               {validConnectionManagerDescription},
                                               {createConnectionManagerSCPDWithoutStateVariable(SinkProtocolInfo)}};

    QTest::newRow("State variable SinkProtocolInfo missing")
        << SinkProtocolInfo_Missing << "ConnectionManager.*SinkProtocolInfo";

    DeviceDescription CurrentConnectionIDs_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(CurrentConnectionIDs)}};

    QTest::newRow("State variable CurrentConnectionIDs missing")
        << CurrentConnectionIDs_Missing << "ConnectionManager.*CurrentConnectionIDs";

    DeviceDescription A_ARG_TYPE_ConnectionStatus_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionStatus)}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionStatus missing")
        << A_ARG_TYPE_ConnectionStatus_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionStatus";

    DeviceDescription A_ARG_TYPE_ConnectionManager_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionManager)}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionManager missing")
        << A_ARG_TYPE_ConnectionManager_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionManager";

    DeviceDescription A_ARG_TYPE_Direction_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_Direction)}};

    QTest::newRow("State variable A_ARG_TYPE_Direction missing")
        << A_ARG_TYPE_Direction_Missing << "ConnectionManager.*A_ARG_TYPE_Direction";

    DeviceDescription A_ARG_TYPE_ProtocolInfo_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ProtocolInfo)}};

    QTest::newRow("State variable A_ARG_TYPE_ProtocolInfo missing")
        << A_ARG_TYPE_ProtocolInfo_Missing << "ConnectionManager.*A_ARG_TYPE_ProtocolInfo";

    DeviceDescription A_ARG_TYPE_ConnectionID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionID)}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionID missing")
        << A_ARG_TYPE_ConnectionID_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionID";

    DeviceDescription A_ARG_TYPE_RcsID_Missing{"",
                                               "",
                                               "",
                                               "",
                                               "",
                                               QVector<IconDescription>{},
                                               {validConnectionManagerDescription},
                                               {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_RcsID)}};

    QTest::newRow("State variable A_ARG_TYPE_RcsID missing")
        << A_ARG_TYPE_RcsID_Missing << "ConnectionManager.*A_ARG_TYPE_RcsID";
}

void MediaDeviceShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaDeviceShould::Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription GetProtocolInfo_Missing{"",
                                              "",
                                              "",
                                              "",
                                              "",
                                              QVector<IconDescription>{},
                                              {validConnectionManagerDescription},
                                              {createConnectionManagerSCPDWithoutAction(GetProtocolInfo)}};

    QTest::newRow("Action GetProtocolInfo missing") << GetProtocolInfo_Missing << "ConnectionManager.*GetProtocolInfo";

    DeviceDescription GetCurrentConnectionIDs_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutAction(GetCurrentConnectionIDs)}};

    QTest::newRow("Action GetCurrentConnectionIDs missing")
        << GetCurrentConnectionIDs_Missing << "ConnectionManager.*GetCurrentConnectionIDs";

    DeviceDescription GetCurrentConnectionInfo_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription},
        {createConnectionManagerSCPDWithoutAction(GetCurrentConnectionInfo)}};

    QTest::newRow("Action GetCurrentConnectionInfo missing")
        << GetCurrentConnectionInfo_Missing << "ConnectionManager.*GetCurrentConnectionInfo";
}

void MediaDeviceShould::Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try
    {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    }
    catch (const InvalidDeviceDescription &e)
    {
        QVERIFY2(QString{e.what()}.contains(QRegExp(ExpectedException)),
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
    }
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetProtocolInfo()
{
    auto device = MediaDeviceWithoutAV{};
    const auto expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetProtocolInfo xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"/>"
                "</s:Body>"
                "</s:Envelope>"};

    device.protocolInfo();

    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionIds()
{
    auto device = MediaDeviceWithoutAV{};
    const auto expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetCurrentConnectionIDs xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"/>"
                "</s:Body>"
                "</s:Envelope>"};
    device.currentConnectionIds();
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionInfo()
{
    auto device = MediaDeviceWithoutAV{};
    const auto expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetCurrentConnectionInfo xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\">"
                "<u:ConnectionID>2</u:ConnectionID>"
                "</u:GetCurrentConnectionInfo>"
                "</s:Body>"
                "</s:Envelope>"};

    device.currentConnectionInfo(2);
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaDeviceShould)
