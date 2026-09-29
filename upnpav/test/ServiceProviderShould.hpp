// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVERPROVIDERSHOULD_H
#define MEDIASERVERPROVIDERSHOULD_H

#include <QObject>
#include <QSharedPointer>
#include <memory>

class QNetworkDatagram;

namespace UPnPAV
{
class IServiceProvider;
class TestableMediaServerProviderFactory;

class ServiceProviderShould final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderShould)
public:
    ServiceProviderShould() noexcept;
    ~ServiceProviderShould() override;

private:
    static QNetworkDatagram createServiceDiscoveryRequestMessage(QString const& message);
    static QNetworkDatagram createServiceDiscoveryReceiveMessage(QString const& message);

private Q_SLOTS:
    /**
     * Initialize variables for every test.
     */
    void init();

    /**
     * @test The MediaServerProvider creates the correct M-Search message format when the user
     *       calls the startSearch().
     */
    void send_out_correct_discovery_message();

    /**
     * @test The ServiceProvider shall request the device description when the answer of a search
     *       request is received. M-Search responses are destination specific and have a little different
     *       format then alive messages.
     */
    void request_device_description_on_received_search_response();

    /**
     * @tets The MediaServerProvider shall handle normal notify messages and reads the description from
     *       it.
     */
    void handle_normal_notify_messages();

    /**
     * @test The ServiceProvider shall handle a search response of a device only once even when the device
     *       sends more then once.
     */
    void handle_every_search_response_from_a_device_only_once();

    /**
     * @test The ServiceProvider shall handle ssdp::bye message.
     */
    void handle_sddp_bye_messages_and_inform_clients_about_the_disconnect();

    /**
     * @test The ServiceProvider shall report the end of a search when the MX window of the search request is over.
     */
    void report_the_end_of_a_search_after_the_mx_window();

    /**
     * @test The end of a search shall be reported while device announcements are expiring as well.
     */
    void report_the_end_of_a_search_while_devices_are_known();

    /**
     * @test The ServiceProvider shall report a device as disconnected when it isn't re-announced
     *       within the max-age of its last announcement.
     */
    void report_a_device_as_disconnected_when_its_announcement_expires();

    /**
     * @test Every announcement of a device shall restart its expiry.
     */
    void extend_the_expiry_when_a_device_is_announced_again();

    /**
     * @test An expired device shall be reported as disconnected only once, a later ssdp:byebye is ignored.
     */
    void report_an_expired_device_as_disconnected_only_once();

    /**
     * @test A device that said ssdp:byebye shall not be reported again when its announcement expires.
     */
    void not_report_a_device_that_said_byebye_again_when_its_announcement_expires();

    /**
     * @test An expired device that is announced again shall be handled like a new device.
     */
    void request_the_device_description_again_when_an_expired_device_is_announced();

    /**
     * @test Every search response of a device shall restart its expiry.
     */
    void extend_the_expiry_when_a_device_answers_a_search_again();

    /**
     * @test An expired device that is announced again shall be reported as connected again.
     */
    void report_an_expired_device_as_connected_again_when_it_is_announced();
    void report_a_disconnected_device_as_connected_again_when_it_is_announced();
    void not_report_an_unknown_device_as_disconnected();

    /**
     * @test A max-age that can't be represented shall be treated as not valid.
     */
    void not_expire_devices_with_an_out_of_range_max_age();

    /**
     * @test Devices whose announcement has no valid max-age shall never expire.
     */
    void not_expire_devices_without_a_valid_max_age();

    /**
     * @test The ServiceProvider should ignore SSDP messages when the have the wrong dest ip
     *       and port because then they may not received from the network.
     */
    void ignore_message_with_wrong_ip_and_port_notify_error();

    /**
     * @test The ServiceProvider should ignore messages when they are empty and
     * noitfy the clients the error signal and error object.
     */
    void ignore_message_with_no_payload_and_notify_clients();

    /**
     * @test The ServiceProvider should report an error when the location entry is
     * not part of the notify message.
     */
    void report_error_when_in_message_location_is_missing_and_no_byebye_type();

    /**
     * @test The ServiceProvider should parse the device description and reports
     * the DeviceDescription, when the device contains the required service.
     */
    void parse_device_description_without_services_no_embedded_services_and_return_values();

    /**
     * @test The ServiceProvider should parse the device description and report the
     * DeviceDescription for the required service.
     */
    void parse_device_description_with_embedded_devices_without_services();

    /**
     * @test The ServiceProvider should parse the device description with a empty
     * URLbase entry. The ServiceProvider then shall use the Url where it fetched
     * the device description from.
     */
    void parse_device_description_with_empty_base_url();

    /**
     * @test The ServiceProvider should parse the device desciption when the base
     * url is defined after the <device> element in the xml.
     */
    void parse_device_description_base_url_after_device_description();

    /**
     * @test The ServiceProvider should parse the serivce descriptions in the
     * device description. Also for embedded devices.
     */
    void parse_devices_description_with_services();

    /**
     * @test The ServiceProvider should derive the address of a device from the location of its
     * device description.
     */
    void derive_the_device_address_from_the_description_location();

    /**
     * @test The ServiceProvider should keep the address of a device when the service control point
     * definitions of its services are fetched.
     */
    void keep_the_device_address_after_fetching_the_service_definitions();

    /**
     * @test The ServiceProvider should give embedded devices the address of the description location.
     */
    void derive_the_address_of_an_embedded_device_from_the_description_location();

    /**
     * @test The ServiceProvider should repot an error when the received xml
     * can't be parsed entirely.
     */
    void ignore_broken_device_description_notify_error();

    /**
     * @test The ServiceProvider should request the SCPD description XML.
     */
    void request_scpd_configurations();

    /**
     * @test The ServiceProvider should request SCPD of the device the
     * corresponding service control point definiton and add them to the root device
     * description.
     */
    void parse_service_control_point_definition();

    /**
     * @test The ServiceProvider shall ignore discovery messages from devices that
     * doesn t match the search target.
     */
    void do_not_request_device_description_for_devices_that_not_match_search_target();

private:
    std::unique_ptr<IServiceProvider> m_mediaServerProvider;
    QSharedPointer<TestableMediaServerProviderFactory> m_providerFactory;
};

} // namespace UPnPAV

#endif // MEDIASERVERPROVIDERSHOULD_H
