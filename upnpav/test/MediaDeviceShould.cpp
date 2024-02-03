// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaDeviceShould.hpp"
#include "AvTransportActions.hpp"
#include "AvTransportEventNotifies.hpp"
#include "AvTransportStateVariables.hpp"
#include "ConnectionManagerActions.hpp"
#include "ConnectionManagerStateVariables.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "EventBackendDouble.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaDevice.hpp"
#include "SCPDAction.hpp"
#include "SCPDStateVariable.hpp"
#include "SoapBackendDouble.hpp"
#include <QSignalSpy>
#include <QTest>

namespace UPnPAV
{

class TestMediaDevice : public MediaDevice
{
public:
    TestMediaDevice(DeviceDescription deviceDescription,
                    QSharedPointer<SoapBackendDouble> msgTransmitter,
                    QSharedPointer<Doubles::EventBackend> eventBackend)
        : MediaDevice{deviceDescription, msgTransmitter, eventBackend}
        , mMsgTransmitter{msgTransmitter}
        , mEventBackend{eventBackend}
    {
    }

    QString lastSoapCall() const noexcept
    {
        return mMsgTransmitter->xmlMessageBody();
    }

    QSharedPointer<Doubles::EventBackend> const& eventBackend() const noexcept
    {
        return mEventBackend;
    }

protected:
    QSharedPointer<SoapBackendDouble> mMsgTransmitter;
    QSharedPointer<Doubles::EventBackend> mEventBackend;
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
                                {validContentDirectoryDescription(), validConnectionManagerDescription()},
                                {validContentDirectorySCPD(), validConnectionManagerSCPD()}},
              QSharedPointer<SoapBackendDouble>::create(),
              QSharedPointer<Doubles::EventBackend>::create()}
    {
    }

    MediaDeviceWithoutAV(DeviceDescription devDesc)
        : TestMediaDevice{devDesc,
                          QSharedPointer<SoapBackendDouble>::create(),
                          QSharedPointer<Doubles::EventBackend>::create()}
    {
    }
};

class MediaDeviceWithAV : public TestMediaDevice
{
public:
    MediaDeviceWithAV()
        : TestMediaDevice{
              DeviceDescription{
                  "",
                  "MediaServerName",
                  "",
                  "",
                  "",
                  QVector<IconDescription>{{"", 0, 0, 24, "http://localhost:8200/icons/sm.png"}},
                  {validContentDirectoryDescription(),
                   validConnectionManagerDescription(),
                   validAvTransportServiceDescription()},
                  {validContentDirectorySCPD(), validConnectionManagerSCPD(), validAvTranportServiceSCPD()}},
              QSharedPointer<SoapBackendDouble>::create(),
              QSharedPointer<Doubles::EventBackend>::create()}
    {
    }

    MediaDeviceWithAV(DeviceDescription devDesc)
        : TestMediaDevice{devDesc,
                          QSharedPointer<SoapBackendDouble>::create(),
                          QSharedPointer<Doubles::EventBackend>::create()}
    {
    }
};

MediaDeviceShould::MediaDeviceShould()
    : QObject()
{
}

MediaDeviceShould::~MediaDeviceShould() = default;

ServiceControlPointDefinition MediaDeviceShould::createConnectionManagerSCPDWithoutStateVariable(
    SCPDStateVariable const& variable)
{
    QVector<SCPDStateVariable> variables = validConnectionManagerStateVariables();
    variables.removeAll(variable);

    return ServiceControlPointDefinition{"http://127.0.0.1/ConnectionManager.xml",
                                         variables,
                                         validConnectionManagerActions()};
}

ServiceControlPointDefinition MediaDeviceShould::createAvTransportSCPDWithoutStateVariable(
    SCPDStateVariable const& variable)
{
    QVector<SCPDStateVariable> variables = validAvTransportStateVariables();
    variables.removeAll(variable);
    return ServiceControlPointDefinition{"http://127.0.0.1/AVTransport.xml", variables, validAvTranportActions()};
}

ServiceControlPointDefinition MediaDeviceShould::createAvTransportSCPDWithoutAction(SCPDAction const& action)
{
    auto actions = validAvTranportActions();
    actions.removeAll(action);
    return ServiceControlPointDefinition{"http://127.0.0.1/AVTransport.xml", validAvTransportStateVariables(), actions};
}

DeviceDescription MediaDeviceShould::createAvTransportDeviceDescriptionWithoutStateVariable(
    SCPDStateVariable const& variable)
{
    DeviceDescription devDesc{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        QVector<ServiceDescription>{validContentDirectoryDescription(),
                                    validConnectionManagerDescription(),
                                    validAvTransportServiceDescription()},
        QVector<ServiceControlPointDefinition>{validContentDirectorySCPD(),
                                               validConnectionManagerSCPD(),
                                               createAvTransportSCPDWithoutStateVariable(variable)}};
    return devDesc;
}

DeviceDescription MediaDeviceShould::createAvTransportDeviceDescriptionWithoutAction(SCPDAction const& action)
{
    DeviceDescription devDesc{"",
                              "",
                              "",
                              "",
                              "",
                              QVector<IconDescription>{},
                              QVector<ServiceDescription>{validContentDirectoryDescription(),
                                                          validConnectionManagerDescription(),
                                                          validAvTransportServiceDescription()},
                              QVector<ServiceControlPointDefinition>{validContentDirectorySCPD(),
                                                                     validConnectionManagerSCPD(),
                                                                     createAvTransportSCPDWithoutAction(action)}};
    return devDesc;
}

ServiceControlPointDefinition MediaDeviceShould::createConnectionManagerSCPDWithoutAction(SCPDAction const& action)
{
    QVector<SCPDAction> actions = validConnectionManagerActions();
    actions.removeAll(action);

    return ServiceControlPointDefinition{"http://127.0.0.1/ConnectionManager.xml",
                                         validConnectionManagerStateVariables(),
                                         actions};
}

void MediaDeviceShould::throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription()
{
    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"", "", "", "", "", {}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager description not found"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url()
{
    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {eventUrlMissingInConnectionManagerDescription()},
                                                                {validConnectionManagerSCPD()}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager event URL is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_ServiceId()
{
    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {serviceIdMissingInConnectionManagerDescription()},
                                                                {validConnectionManagerSCPD()}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager service ID is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url()
{
    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {scpdUrlMissingInConnectionManagerDescription()},
                                                                {validConnectionManagerSCPD()}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD URL is not set"));
    }
}

void MediaDeviceShould::throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager()
{
    try {
        auto const dev =
            MediaDeviceWithoutAV(DeviceDescription{"", "", "", "", "", {}, {validConnectionManagerDescription()}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager SCPD not found."));
    }
}

void MediaDeviceShould::throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url()
{
    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription{"",
                                                                "",
                                                                "",
                                                                "",
                                                                "",
                                                                {},
                                                                {controlUrlMissingInConnectionManagerDescription()},
                                                                {validConnectionManagerSCPD()}});
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("ConnectionManager control URL is not set"));
    }
}

void MediaDeviceShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription SourceProtocolInfo_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(SourceProtocolInfo())}};

    QTest::newRow("State variable SourceProtocolInfo missing")
        << SourceProtocolInfo_Missing << "ConnectionManager.*SourceProtocolInfo";

    DeviceDescription SinkProtocolInfo_Missing{"",
                                               "",
                                               "",
                                               "",
                                               "",
                                               QVector<IconDescription>{},
                                               {validConnectionManagerDescription()},
                                               {createConnectionManagerSCPDWithoutStateVariable(SinkProtocolInfo())}};

    QTest::newRow("State variable SinkProtocolInfo missing")
        << SinkProtocolInfo_Missing << "ConnectionManager.*SinkProtocolInfo";

    DeviceDescription CurrentConnectionIDs_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(CurrentConnectionIDs())}};

    QTest::newRow("State variable CurrentConnectionIDs missing")
        << CurrentConnectionIDs_Missing << "ConnectionManager.*CurrentConnectionIDs";

    DeviceDescription A_ARG_TYPE_ConnectionStatus_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionStatus())}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionStatus missing")
        << A_ARG_TYPE_ConnectionStatus_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionStatus";

    DeviceDescription A_ARG_TYPE_ConnectionManager_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionManager())}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionManager missing")
        << A_ARG_TYPE_ConnectionManager_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionManager";

    DeviceDescription A_ARG_TYPE_Direction_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_Direction())}};

    QTest::newRow("State variable A_ARG_TYPE_Direction missing")
        << A_ARG_TYPE_Direction_Missing << "ConnectionManager.*A_ARG_TYPE_Direction";

    DeviceDescription A_ARG_TYPE_ProtocolInfo_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ProtocolInfo())}};

    QTest::newRow("State variable A_ARG_TYPE_ProtocolInfo missing")
        << A_ARG_TYPE_ProtocolInfo_Missing << "ConnectionManager.*A_ARG_TYPE_ProtocolInfo";

    DeviceDescription A_ARG_TYPE_ConnectionID_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_ConnectionID())}};

    QTest::newRow("State variable A_ARG_TYPE_ConnectionID missing")
        << A_ARG_TYPE_ConnectionID_Missing << "ConnectionManager.*A_ARG_TYPE_ConnectionID";

    DeviceDescription A_ARG_TYPE_RcsID_Missing{"",
                                               "",
                                               "",
                                               "",
                                               "",
                                               QVector<IconDescription>{},
                                               {validConnectionManagerDescription()},
                                               {createConnectionManagerSCPDWithoutStateVariable(A_ARG_TYPE_RcsID())}};

    QTest::newRow("State variable A_ARG_TYPE_RcsID missing")
        << A_ARG_TYPE_RcsID_Missing << "ConnectionManager.*A_ARG_TYPE_RcsID";
}

void MediaDeviceShould::throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        // clang-format off
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
        // clang-format on
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
                                              {validConnectionManagerDescription()},
                                              {createConnectionManagerSCPDWithoutAction(GetProtocolInfo())}};

    QTest::newRow("Action GetProtocolInfo missing") << GetProtocolInfo_Missing << "ConnectionManager.*GetProtocolInfo";

    DeviceDescription GetCurrentConnectionIDs_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutAction(GetCurrentConnectionIDs())}};

    QTest::newRow("Action GetCurrentConnectionIDs missing")
        << GetCurrentConnectionIDs_Missing << "ConnectionManager.*GetCurrentConnectionIDs";

    DeviceDescription GetCurrentConnectionInfo_Missing{
        "",
        "",
        "",
        "",
        "",
        QVector<IconDescription>{},
        {validConnectionManagerDescription()},
        {createConnectionManagerSCPDWithoutAction(GetCurrentConnectionInfo())}};

    QTest::newRow("Action GetCurrentConnectionInfo missing")
        << GetCurrentConnectionInfo_Missing << "ConnectionManager.*GetCurrentConnectionInfo";
}

void MediaDeviceShould::Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        // clang-format off
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
        // clang-format on
    }
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetProtocolInfo()
{
    auto device = MediaDeviceWithoutAV{};
    auto const expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetProtocolInfo xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"/>"
                "</s:Body>"
                "</s:Envelope>"};

    (void)device.protocolInfo();

    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionIds()
{
    auto device = MediaDeviceWithoutAV{};
    auto const expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetCurrentConnectionIDs xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"/>"
                "</s:Body>"
                "</s:Envelope>"};
    (void)device.currentConnectionIds();
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionInfo()
{
    auto device = MediaDeviceWithoutAV{};
    auto const expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetCurrentConnectionInfo xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\">"
                "<ConnectionID>2</ConnectionID>"
                "</u:GetCurrentConnectionInfo>"
                "</s:Body>"
                "</s:Envelope>"};

    (void)device.currentConnectionInfo(2);
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::Have_A_Check_For_The_Existence_Of_A_AVTransportService()
{
    auto deviceWithoutAV = MediaDeviceWithoutAV{};

    QVERIFY2(deviceWithoutAV.hasAvTransportService() == false,
             QString("The device should not have a AVTransport service.").toLocal8Bit());

    auto deviceWithAV = MediaDeviceWithAV{};

    QVERIFY2(deviceWithAV.hasAvTransportService() == true,
             QString("The device should have a AVTransport service.").toLocal8Bit());
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Is_Not_Correct_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    // clang-format off
    DeviceDescription eventUrl_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        QVector<ServiceDescription>{
            validContentDirectoryDescription(),
            validConnectionManagerDescription(),
            eventUrlMissingInAvTransportDescription()
        },
        QVector<ServiceControlPointDefinition>{
            validContentDirectorySCPD(),
            validConnectionManagerSCPD(),
            validAvTranportServiceSCPD()
        }
    };

    QTest::newRow("Event Url missing in AVTransport service description")
        << eventUrl_Missing
        << "AVTransport event URL is not set";

    DeviceDescription controlUrl_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        QVector<ServiceDescription>{
            validContentDirectoryDescription(),
            validConnectionManagerDescription(),
            controlUrlMissingInAvTransportDescription()
        },
        QVector<ServiceControlPointDefinition>{
            validContentDirectorySCPD(),
            validConnectionManagerSCPD(),
            validAvTranportServiceSCPD()
        }
    };

    QTest::newRow("Control Url missing in AVTransport service description")
        << controlUrl_Missing
        << "AVTransport control URL is not set";

    DeviceDescription serviceUrl_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        QVector<ServiceDescription>{
            validContentDirectoryDescription(),
            validConnectionManagerDescription(),
            serviceUrlMissingInAvTransportDescription()
        },
        QVector<ServiceControlPointDefinition>{
            validContentDirectorySCPD(),
            validConnectionManagerSCPD(),
            validAvTranportServiceSCPD()
        }
    };

    QTest::newRow("Service Url missing in AVTransport service description")
        << serviceUrl_Missing
        << "AVTransport service ID is not set";

    DeviceDescription scpdUrl_Missing
    {
        "", "", "", "", "",
        QVector<IconDescription>{},
        QVector<ServiceDescription>{
            validContentDirectoryDescription(),
            validConnectionManagerDescription(),
            scpdUrlMissingInAvTransportDescription()
        },
        QVector<ServiceControlPointDefinition>{
            validContentDirectorySCPD(),
            validConnectionManagerSCPD(),
            validAvTranportServiceSCPD()
        }
    };

    QTest::newRow("SCPD Url missing in AVTransport service description")
        << scpdUrl_Missing
        << "AVTransport SCPD URL is not set";
    // clang-format on
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Is_Not_Correct()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try {
        auto const device = MediaDeviceWithAV{DeviceDescription};
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        // clang-format off
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
        // clang-format on
    }
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Variable_Is_Not_Correct_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    DeviceDescription transportStateMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createTransportStateVariable());
    QTest::newRow("AVTransport service state variable TransportState missing")
        << transportStateMissing << ".*TransportState.*";

    DeviceDescription transportStatusMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createTransportStatusVariable());
    QTest::newRow("AVTransport service state variable TransportStatus missing")
        << transportStatusMissing << ".*TransportStatus.*";

    DeviceDescription playbackStorageMediumMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createPlaybackStorageMediumVariable());
    QTest::newRow("AVTransport service state variable PlaybackStorageMedium missing")
        << playbackStorageMediumMissing << ".*PlaybackStorageMedium.*";

    DeviceDescription recordStorageMediumMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createRecordStorageMediumVariable());
    QTest::newRow("AVTransport service state variable RecordStorageMedium missing")
        << recordStorageMediumMissing << ".*RecordStorageMedium.*";

    DeviceDescription possiblePlaybackStorageMedium =
        createAvTransportDeviceDescriptionWithoutStateVariable(createPossiblePlaybackStorageMediaVariable());
    QTest::newRow("AVTransport service state variable PossiblePlaybackStorageMedia missing")
        << possiblePlaybackStorageMedium << ".*PossiblePlaybackStorageMedia.*";

    DeviceDescription currentPlayModeMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentPlayModeVariable());
    QTest::newRow("AVTransport service state variable CurrentPlayMode missing")
        << currentPlayModeMissing << ".*CurrentPlayMode.*";

    DeviceDescription transportPlaySpeedMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createTransportPlaySpeedVariable());
    QTest::newRow("AVTransport service state variable TransportPlaySpeed missing")
        << transportPlaySpeedMissing << ".*TransportPlaySpeed.*";

    DeviceDescription recordMediumWriteStatusMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createRecordMediumWriteStatusVariable());
    QTest::newRow("AVTransport service state variable RecordMediumWriteStatus missing")
        << recordMediumWriteStatusMissing << ".*RecordMediumWriteStatus.*";

    DeviceDescription currentRecordQualityModeMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentRecordQualityModeVariable());
    QTest::newRow("AVTransport service state variable CurrentRecordQualityMode missing")
        << currentRecordQualityModeMissing << ".*CurrentRecordQualityMode.*";

    DeviceDescription possibleRecordQualityModesMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createPossibleRecordQualityModesVariable());
    QTest::newRow("AVTransport service state variable PossibileRecordQualityModes missing")
        << possibleRecordQualityModesMissing << ".*PossibleRecordQualityModes.*";

    DeviceDescription numberOfTracksMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createNumberOfTracksVariable());
    QTest::newRow("AVTransport service state variable NumberOfTracks missing")
        << numberOfTracksMissing << ".*NumberOfTracks.*";

    DeviceDescription currentTrackMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentTrackVariable());
    QTest::newRow("AVTransport service state variable CurrentTrack missing")
        << currentTrackMissing << ".*CurrentTrack.*";

    DeviceDescription currentTrackDurationMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentTrackDurationVariable());
    QTest::newRow("AVTransport service state variable CurrentTrackDuration missing")
        << currentTrackDurationMissing << ".*CurrentTrackDuration.*";

    DeviceDescription currentMediaDurationMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentMediaDurationVariable());
    QTest::newRow("AVTransport service state variable CurrentMediaDuration missing")
        << currentMediaDurationMissing << ".*CurrentMediaDuration.*";

    DeviceDescription currentTrackMetaDataMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createCurrentTrackMetaDataVariable());
    QTest::newRow("AVTransport service state variable CurrentTrackMetaData missing")
        << currentTrackMetaDataMissing << ".*CurrentTrackMetaData.*";

    DeviceDescription avTransportUriMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createAVTransportURIVariable());
    QTest::newRow("AVTransport service state variable AVTransportURI missing")
        << avTransportUriMissing << ".*AVTransportURI.*";

    DeviceDescription avTransportUriMetaDataMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createAVTransportURIMetaDataVariable());
    QTest::newRow("AVTransport service state variable AVTransportURIMetaData missing")
        << avTransportUriMetaDataMissing << ".*AVTransportURIMetaData.*";

    DeviceDescription nextAvTransportUriMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createNextAVTransportURIVariable());
    QTest::newRow("AVTransport service state variable NextAVTransportURI missing")
        << nextAvTransportUriMissing << ".* NextAVTransportURI.*";

    DeviceDescription nextAvTransportUriMetaDataMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createNextAVTransportURIMetaDataVariable());
    QTest::newRow("AVTransport service state variable NextAVTransportURIMetaData missing")
        << nextAvTransportUriMetaDataMissing << ".* NextAVTransportURIMetaData.*";

    DeviceDescription relativeTimePositionMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createRelativeTimePositionVariable());
    QTest::newRow("AVTransport service state variable RelativeTimePosition missing")
        << relativeTimePositionMissing << ".*RelativeTimePosition.*";

    DeviceDescription absoluteTimePositionMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createAbsoluteTimePositionVariable());
    QTest::newRow("AVTransport service state variable AbsoluteTimePosition missing")
        << absoluteTimePositionMissing << ".*AbsoluteTimePosition.*";

    DeviceDescription relativeCounterPositionMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createRelativeCounterPositionVariable());
    QTest::newRow("AVTransport service state variable RelativeCounterPosition missing")
        << relativeCounterPositionMissing << ".*RelativeCounterPosition.*";

    DeviceDescription absoluteCounterPositionMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createAbsoluteCounterPositionVariable());
    QTest::newRow("AVTransport service state variable AbsoluteCounterPosition missing")
        << absoluteCounterPositionMissing << ".*AbsoluteCounterPosition.*";

    DeviceDescription lastChangeMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createLastChangeVariable());
    QTest::newRow("AVTransport service state variable LastChange missing") << lastChangeMissing << ".*LastChange.*";

    DeviceDescription seekModeMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createA_ARG_TYPE_SeekModeVariable());
    QTest::newRow("AVTransport service state variable A_ARG_TYPE_SeekMode missing")
        << seekModeMissing << ".*A_ARG_TYPE_SeekMode.*";

    DeviceDescription seekTargetMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createA_ARG_TYPE_SeekTargetVariable());
    QTest::newRow("AVTransport service state variable A_ARG_TYPE_SeekTarget missing")
        << seekTargetMissing << ".*A_ARG_TYPE_SeekTarget.*";

    DeviceDescription instanceIdMissing =
        createAvTransportDeviceDescriptionWithoutStateVariable(createA_ARG_TYPE_InstanceIDVariable());
    QTest::newRow("AVTransport service state variable A_ARG_TYPE_InstanceID missing")
        << instanceIdMissing << ".*A_ARG_TYPE_InstanceID.*";
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Variable_Is_Not_Correct()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        // clang-format off
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
        // clang-format on
    }
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Action_Is_Not_Correct_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    QTest::addColumn<QString>("ExpectedException");

    auto avTransportUri = createAvTransportDeviceDescriptionWithoutAction(createSetAVTransportURIAction());
    QTest::newRow("AVTransport service state variable SetAVTransportURI missing")
        << avTransportUri << ".*SetAVTransportURI.*";

    auto getMediaInfo = createAvTransportDeviceDescriptionWithoutAction(createGetMediaInfoAction());
    QTest::newRow("AVTransport service state variable GetMediaInfo missing") << getMediaInfo << ".*GetMediaInfo.*";

    auto getTransportInfo = createAvTransportDeviceDescriptionWithoutAction(createGetTransportInfoAction());
    QTest::newRow("AVTransport service state variable GetTransportInfo missing")
        << getTransportInfo << ".*GetTransportInfo.*";

    auto getPositionInfo = createAvTransportDeviceDescriptionWithoutAction(createGetPositionInfoAction());
    QTest::newRow("AVTransport service state variable GetPositionInfo missing")
        << getPositionInfo << ".*GetPositionInfo.*";

    auto getDeviceCapabilities = createAvTransportDeviceDescriptionWithoutAction(createGetDeviceCapabilitiesAction());
    QTest::newRow("AVTransport service state variable GetDeviceCapabilities missing")
        << getDeviceCapabilities << ".*GetDeviceCapabilities.*";

    auto getTransportSettings = createAvTransportDeviceDescriptionWithoutAction(createGetTransportSettingsAction());
    QTest::newRow("AVTransport service state variable GetTransportSettings missing")
        << getTransportSettings << ".*GetTransportSettings.*";

    auto stop = createAvTransportDeviceDescriptionWithoutAction(createStopAction());
    QTest::newRow("AVTransport service state variable Stop missing") << stop << ".*Stop.*";

    auto play = createAvTransportDeviceDescriptionWithoutAction(createPlayAction());
    QTest::newRow("AVTransport service state variable Play missing") << play << ".*Play.*";

    auto seek = createAvTransportDeviceDescriptionWithoutAction(createSeekAction());
    QTest::newRow("AVTransport service state variable Seek missing") << seek << ".*Seek.*";

    auto next = createAvTransportDeviceDescriptionWithoutAction(createNextAction());
    QTest::newRow("AVTransport service state variable Next missing") << next << ".*Next.*";

    auto previous = createAvTransportDeviceDescriptionWithoutAction(createPreviousAction());
    QTest::newRow("AVTransport service state variable Previous missing") << previous << ".*Previous.*";
}

void MediaDeviceShould::Throw_An_Exception_When_The_AVTransport_Service_Description_Action_Is_Not_Correct()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QFETCH(QString, ExpectedException);

    try {
        auto const dev = MediaDeviceWithoutAV(DeviceDescription);
        QFAIL("The consturctor should throw Invalid Device Description.");
    } catch (InvalidDeviceDescription const& e) {
        // clang-format off
        const auto re = QRegularExpression(ExpectedException);
        QVERIFY2(QString{e.what()}.contains(re), // clazy:exclude=use-static-qregularexpression
                 QString{"Actual:"}.append(e.what()).toLocal8Bit());
        // clang-format on
    }
}

void MediaDeviceShould::Send_The_Correct_SOAP_Message_When_Calling_SetAVTransportUri()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "<CurrentURI>http://someUri.com/file.mp3</CurrentURI>"
                                         "<CurrentURIMetaData>audio/mp3</CurrentURIMetaData>"
                                         "</u:SetAVTransportURI>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    device.setAvTransportUri(2, QStringLiteral("http://someUri.com/file.mp3"), QStringLiteral("audio/mp3"));

    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_getmediainfo()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:GetMediaInfo xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:GetMediaInfo>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    (void)device.mediaInfo(2);

    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_gettransportinfo()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:GetTransportInfo xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:GetTransportInfo>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    (void)device.transportInfo(2);

    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_getpositioninfo()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:GetPositionInfo xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:GetPositionInfo>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.positionInfo(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_getdevicecapabilities()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetDeviceCapabilities xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                "<InstanceID>2</InstanceID>"
                "</u:GetDeviceCapabilities>"
                "</s:Body>"
                "</s:Envelope>"};

    auto call = device.deviceCapilities(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_gettransportsettings()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage =
        QString{"<?xml version=\"1.0\"?>"
                "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                "<s:Body>"
                "<u:GetTransportSettings xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                "<InstanceID>2</InstanceID>"
                "</u:GetTransportSettings>"
                "</s:Body>"
                "</s:Envelope>"};

    auto call = device.transportSettings(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_stop()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:Stop xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:Stop>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.stop(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_play()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "<Speed>1</Speed>"
                                         "</u:Play>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.play(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_seek_with_int_target()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:Seek xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "<Unit>1</Unit>"
                                         "<Target>12</Target>"
                                         "</u:Seek>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.seek(2, MediaDevice::SeekMode::RelTime, QStringLiteral("12"));

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_next()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:Next xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:Next>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.next(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::send_the_correct_soap_message_when_calling_previous()
{
    auto device = MediaDeviceWithAV{};
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:Previous xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "</u:Previous>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto call = device.previous(2);

    QVERIFY2(call.has_value(),
             QString{"The media device has an AVTransportService and the call must have a Value"}.toLocal8Bit());
    QVERIFY2(device.lastSoapCall() == QString{expectedMessage},
             QString("The send SOAP message \n %1 \n is not the same as the expected \n %2")
                 .arg(device.lastSoapCall().toLocal8Bit(), QString{expectedMessage}.toLocal8Bit())
                 .toLocal8Bit());
}

void MediaDeviceShould::subscribe_events_of_avtransport_service_on_creation()
{
    auto const expectParams = EventSubscriptionParameters{
        .publisherPath = QStringLiteral("/test/eventUrl"),
        .host = "127.0.0.1:27016",
        .callback = QStringLiteral("<http://127.0.0.1/AVTransportCallback>"),
        .timeout = 1800,
    };
    auto const expCbId = QStringLiteral("/AVTransportCallback");
    auto mediaDevice = MediaDeviceWithAV{};

    QCOMPARE(mediaDevice.eventBackend()->lastRegisteredCallbackId(), expCbId);
    QCOMPARE(mediaDevice.eventBackend()->lastSubscribeEventRequest(), expectParams);
}

void MediaDeviceShould::set_device_state_reported_by_the_av_transport_service_data()
{
    QTest::addColumn<QString>("TransportState");
    QTest::addColumn<MediaDevice::State>("ExpectedState");
    QTest::addColumn<qsizetype>("StateChanged");

    QTest::addRow("Stopped") << "STOPPED" << MediaDevice::State::Stopped << qsizetype{1};
    QTest::addRow("PausedPlayback") << "PAUSED_PLAYBACK" << MediaDevice::State::PausedPlayback << qsizetype{1};
    QTest::addRow("PausedRecording") << "PAUSED_RECORDING" << MediaDevice::State::PausedRecording << qsizetype{1};
    QTest::addRow("Playing") << "PLAYING" << MediaDevice::State::Playing << qsizetype{1};
    QTest::addRow("Recording") << "RECORDING" << MediaDevice::State::Recording << qsizetype{1};
    QTest::addRow("Transitioning") << "TRANSITIONING" << MediaDevice::State::Transitioning << qsizetype{1};
    QTest::addRow("NoMediaPresent") << "NO_MEDIA_PRESENT" << MediaDevice::State::NoMediaPresent << qsizetype{0};
}

void MediaDeviceShould::set_device_state_reported_by_the_av_transport_service()
{
    QFETCH(QString, TransportState);
    QFETCH(MediaDevice::State, ExpectedState);
    QFETCH(qsizetype, StateChanged);
    auto mediaDevice = MediaDeviceWithAV{};
    auto eventHandle = mediaDevice.eventBackend()->subscribeEvents(validAvTransportServiceDescription());
    auto handle = std::dynamic_pointer_cast<UPnPAV::Doubles::EventSubscriptionHandle>(eventHandle);
    QCOMPARE_NE(handle, nullptr);
    auto stateChangedSpy = QSignalSpy{&mediaDevice, &MediaDevice::stateChanged};

    handle->sendNotifyBody(QString{UPnPAV::xmlNotify}.arg(TransportState));

    QCOMPARE(mediaDevice.state(), ExpectedState);
    QCOMPARE(stateChangedSpy.size(), StateChanged);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaDeviceShould)
