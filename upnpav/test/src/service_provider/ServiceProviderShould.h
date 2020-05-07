/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef MEDIASERVERPROVIDERSHOULD_H
#define MEDIASERVERPROVIDERSHOULD_H

#include <QSharedPointer>
#include <QObject>

class QNetworkDatagram;

namespace UPnPAV
{
class ServiceProvider;
class TestableMediaServerProviderFactory;

class ServiceProviderShould final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderShould)
public:
    ServiceProviderShould() noexcept;
    ~ServiceProviderShould() override;

private:
    static QNetworkDatagram createServiceDiscoveryRequestMessage(const QString &message);
    static QNetworkDatagram createServiceDiscoveryReceiveMessage(const QString &message);

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

private:
    QSharedPointer<ServiceProvider> m_mediaServerProvider;
    QSharedPointer<TestableMediaServerProviderFactory> m_providerFactory;
};

} //namespace UPnPAV

#endif // MEDIASERVERPROVIDERSHOULD_H
