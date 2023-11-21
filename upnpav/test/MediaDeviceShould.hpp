// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef UPNPAV_MEDIADEVICEBASETEST_H
#define UPNPAV_MEDIADEVICEBASETEST_H

#include <QObject>
#include <memory>

namespace UPnPAV
{
class ServiceControlPointDefinition;
class SCPDStateVariable;
class SCPDAction;
class DeviceDescription;

/**
 * This class defines some tests for requirements that must be fullfilled by any MediaDevice implementation.
 */
class MediaDeviceShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaDeviceShould)
public:
    MediaDeviceShould();
    ~MediaDeviceShould() override;

private:
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutStateVariable(const SCPDStateVariable &variable);
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutAction(const SCPDAction &action);

    ServiceControlPointDefinition createAvTransportSCPDWithoutStateVariable(const SCPDStateVariable &variable);
    ServiceControlPointDefinition createAvTransportSCPDWithoutAction(SCPDAction const &action);
    DeviceDescription createAvTransportDeviceDescriptionWithoutStateVariable(SCPDStateVariable const &variable);
    DeviceDescription createAvTransportDeviceDescriptionWithoutAction(SCPDAction const &action);

private Q_SLOTS:
    /**
     * @test The media device shall throw an exception when the ConnectionManager Service description
     * is missing.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service
     * has no event URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url();

    /**
     * @test The media device shall throw an exception when the ConnectionMangager service
     * has no control URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service
     * has no service ID set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_ServiceId();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service has no
     * SCPD url set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url();

    /**
     * @test The media device shall throw an exception when the DeviceDescription
     * has no SCPD for the ConnectionManager.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager();

    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data();
    /**
     * @test The media device shall throw an exception on construction when the ConnectionManager SCPD misses one
     * of the minimum required state variables in the device description.
     */
    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD();

    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD_data();
    /**
     * @test The media device shall throw an exception on constructuion when the ConnectionManager SCPD misses one
     * of the minimum required actions in the device description.
     */
    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD();

    /**
     * @test The media device shall send the correct request for the GetProtocolInfo.
     */
    void shall_Send_The_Correct_SOAP_Message_When_Calling_GetProtocolInfo();

    /**
     * @test The media device shall send the correct request for GetCurrentConnectionIds.
     */
    void shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionIds();

    /**
     * @test The media device shall send the correct request for GetCurrentConnectionInfo.
     */
    void shall_Send_The_Correct_SOAP_Message_When_Calling_GetCurrentConnectionInfo();

    /**
     * @test The media device shall have a check for the existence of a AVTransport service.
     */
    void Have_A_Check_For_The_Existence_Of_A_AVTransportService();

    void Throw_An_Exception_When_The_AVTransport_Service_Description_Is_Not_Correct_data();
    /**
     * @test The media should throw an exception when the Service Description is not correct.
     */
    void Throw_An_Exception_When_The_AVTransport_Service_Description_Is_Not_Correct();

    void Throw_An_Exception_When_The_AVTransport_Service_Description_Variable_Is_Not_Correct_data();
    /**
     * @test The media should throw an execption when the AVTransport state variable is missing
     */
    void Throw_An_Exception_When_The_AVTransport_Service_Description_Variable_Is_Not_Correct();

    void Throw_An_Exception_When_The_AVTransport_Service_Description_Action_Is_Not_Correct_data();
    /**
     * @test The media should throw an execption when the AVTransport action is missing
     */
    void Throw_An_Exception_When_The_AVTransport_Service_Description_Action_Is_Not_Correct();

    /**
     * @test The media device should send the correct SOAP for the SetAVTransportURI call;
     */
    void Send_The_Correct_SOAP_Message_When_Calling_SetAVTransportUri();

    void send_the_correct_soap_message_when_calling_getmediainfo();
    void send_the_correct_soap_message_when_calling_gettransportinfo();
    void send_the_correct_soap_message_when_calling_getpositioninfo();
    void send_the_correct_soap_message_when_calling_getdevicecapabilities();
    void send_the_correct_soap_message_when_calling_gettransportsettings();
    void send_the_correct_soap_message_when_calling_stop();
    void send_the_correct_soap_message_when_calling_play();
    void send_the_correct_soap_message_when_calling_seek_with_int_target(); // TODO add tests for float and time
    void send_the_correct_soap_message_when_calling_next();
    void send_the_correct_soap_message_when_calling_previous();
};

} // namespace UPnPAV

#endif // UPNPAV_MEDIADEVICEBASETEST_H
