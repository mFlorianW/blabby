// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef UPNPAV_MEDIADEVICEBASETEST_H
#define UPNPAV_MEDIADEVICEBASETEST_H

#include <QObject>

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
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutStateVariable(SCPDStateVariable const& variable);
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutAction(SCPDAction const& action);

    ServiceControlPointDefinition createAvTransportSCPDWithoutStateVariable(SCPDStateVariable const& variable);
    ServiceControlPointDefinition createAvTransportSCPDWithoutAction(SCPDAction const& action);
    DeviceDescription createAvTransportDeviceDescriptionWithoutStateVariable(SCPDStateVariable const& variable);
    DeviceDescription createAvTransportDeviceDescriptionWithoutAction(SCPDAction const& action);

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

    /**
     * @test
     * Tests that every call is sent with its SCPD action, which is needed to read the out arguments of its answer.
     */
    void send_every_call_with_its_action_to_read_the_answer();
    void send_the_correct_soap_message_when_calling_getdevicecapabilities();
    void send_the_correct_soap_message_when_calling_gettransportsettings();
    void send_the_correct_soap_message_when_calling_stop();
    void send_the_correct_soap_message_when_calling_play();
    void send_the_correct_soap_message_when_calling_seek_data();
    void send_the_correct_soap_message_when_calling_seek();
    void send_the_correct_soap_message_when_calling_next();
    void send_the_correct_soap_message_when_calling_previous();

    /**
     * @test The media device shall subscribe the AVTransport serivce events on device creation
     *       when the device has a AVTransport service.
     */
    void subscribe_events_of_avtransport_service_on_creation();

    void set_device_state_reported_by_the_av_transport_service_data();
    /**
     * @test The media device should set it's state according to the AVTransport service state
     *       reported by the lastChange event update.
     */
    void set_device_state_reported_by_the_av_transport_service();

    /**
     * @test The media device should generate and send the correct soap message
     *       when pause is called.
     */
    void should_send_the_correct_soap_message_when_calling_pause();

    /**
     * @test The media device tells that it's unreachable when the AVTransport event publisher is unreachable.
     */
    void tell_that_it_is_unreachable_when_the_av_transport_event_publisher_is_unreachable();

    /**
     * @test The media device doesn't tell that it's unreachable when a reachable publisher rejects the subscription.
     */
    void not_tell_that_it_is_unreachable_when_the_publisher_rejects_the_subscription();

    /**
     * @test The media device gives the current track reported by the AVTransport service events.
     */
    void give_the_current_track_reported_by_the_av_transport_service();

    /**
     * @test The media device keeps its current track when an event doesn't report it.
     */
    void keep_the_current_track_when_an_event_does_not_report_it();

    /**
     * @test The media device doesn't tell about an unchanged current track.
     */
    void not_notify_about_an_unchanged_current_track();

    /**
     * @test The media device drops the metadata of the previous track when an event reports only a new track URI.
     */
    void drop_the_metadata_of_the_previous_track_for_a_new_track_uri();

    /**
     * @test The media device tells whether its AVTransport service offers Pause.
     */
    void tell_whether_it_can_pause();

    /**
     * @test The media device tells which seek modes its AVTransport service allows.
     */
    void tell_the_allowed_seek_modes();
};

} // namespace UPnPAV

#endif // UPNPAV_MEDIADEVICEBASETEST_H
