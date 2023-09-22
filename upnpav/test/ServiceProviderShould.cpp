// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceProviderShould.h"
#include "DescriptionFetcherBackendDouble.h"
#include "IServiceProvider.h"
#include "ServiceDiscoveryBackendDouble.h"
#include "TestableMediaServerProviderFactory.h"

#include <QNetworkDatagram>
#include <QSharedPointer>
#include <QSignalSpy>
#include <QTest>

namespace UPnPAV
{

namespace
{
constexpr char validMSearchRequest[] = "M-SEARCH * HTTP/1.1\r\n"
                                       "Host: 239.255.255.250:1900\r\n"
                                       "Man: \"ssdp:discover\"\r\n"
                                       "MX: 3\r\n"
                                       "ST: urn:schemas-upnp-org:device:MediaServer:1\r\n"
                                       "USER-AGENT: Linux/1.0 UPnP/1.0 test/0.1.0"
                                       "\r\n";

constexpr char validMSearchResponse[] = "HTTP/1.1 200 OK\r\n"
                                        "CACHE-CONTROL: max-age = seconds until advertisement expires\r\n"
                                        "DATE: when response was generated\r\n"
                                        "EXT:\r\n"
                                        "LOCATION: http://127.0.0.1:8000/desc.xml\r\n"
                                        "SERVER: Linux UPnP/1.0\r\n"
                                        "ST: urn:schemas-upnp-org:device:MediaServer:1\r\n"
                                        "USN: uuid:4d696e69-444c-164e-9d41-b827eb54e939\r\n";

constexpr char validNotifyMessage[] = "NOTIFY * HTTP/1.1\r\n"
                                      "HOST: 239.255.255.250:1900\r\n"
                                      "CACHE-CONTROL: max-age = seconds until advertisement expires \r\n"
                                      "LOCATION: http://127.0.0.1:8000/desc.xml\r\n"
                                      "NT: urn:schemas-upnp-org:device:MediaServer:1\r\n"
                                      "NTS: ssdp:alive\r\n"
                                      "SERVER: OS/version UPnP/1.0 product/version\r\n"
                                      "USN: uuid:4d696e69-444c-164e-9d41-b827eb54e939\r\n";

constexpr char validByeMessage[] = "NOTIFY * HTTP/1.1 \r\n"
                                   "HOST: 239.255.255.250:1900 \r\n"
                                   "NT: urn:schemas-upnp-org:device:MediaServer:1 \r\n"
                                   "NTS: ssdp:byebye \r\n"
                                   "USN: uuid:4d696e69-444c-164e-9d41-b827eb54e939\r\n";
} // namespace

ServiceProviderShould::ServiceProviderShould() noexcept
    : QObject()
{
}

ServiceProviderShould::~ServiceProviderShould()
{
}

QNetworkDatagram ServiceProviderShould::createServiceDiscoveryRequestMessage(const QString &message)
{
    return QNetworkDatagram{message.toUtf8(), QHostAddress{"239.255.255.250"}, 1900};
}

QNetworkDatagram ServiceProviderShould::createServiceDiscoveryReceiveMessage(const QString &message)
{
    return createServiceDiscoveryRequestMessage(message);
}

void ServiceProviderShould::init()
{
    m_providerFactory = QSharedPointer<TestableMediaServerProviderFactory>(new TestableMediaServerProviderFactory{});
    m_mediaServerProvider = m_providerFactory->createServiceProvider("urn:schemas-upnp-org:device:MediaServer:1");
}

void ServiceProviderShould::send_out_correct_discovery_message()
{
    QNetworkDatagram mSearchDatagram = createServiceDiscoveryRequestMessage(validMSearchRequest);

    m_mediaServerProvider->startSearch();

    QNetworkDatagram searchRequest = m_providerFactory->serviceDiscoveryBackendDouble->lastSendDatagram;

    QVERIFY2((mSearchDatagram.destinationPort() == searchRequest.destinationPort()),
             "The destination port is not 1900 in the search request.");
    QVERIFY2((mSearchDatagram.destinationAddress() == searchRequest.destinationAddress()),
             "The destination address is not 239.255.255.250 in the search request");
    QVERIFY2((mSearchDatagram.data() == searchRequest.data()), "The search request playlod is not correctly.");
}

void ServiceProviderShould::request_device_description_on_received_search_response()
{
    QNetworkDatagram mSearchResponse = createServiceDiscoveryReceiveMessage(validMSearchResponse);
    mSearchResponse.setDestination(QHostAddress{QHostAddress::LocalHost}, 40000);
    QUrl expectedDescriptionUrl{"http://127.0.0.1:8000/desc.xml"};
    m_providerFactory->serviceDiscoveryBackendDouble->sendResponseForMediaServerRequest(mSearchResponse);

    auto descriptionUrl = m_providerFactory->descriptionFetcherBackendDouble->lastDescriptionRequest;

    QVERIFY2(expectedDescriptionUrl == descriptionUrl, "The sendout description URL is not expected description URL.");
}

void ServiceProviderShould::handle_normal_notify_messages()
{
    QNetworkDatagram notifyMessage = createServiceDiscoveryReceiveMessage(validNotifyMessage);
    QUrl expectedDescriptionUrl{"http://127.0.0.1:8000/desc.xml"};
    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(notifyMessage);

    auto descriptionUrl = m_providerFactory->descriptionFetcherBackendDouble->lastDescriptionRequest;

    QVERIFY2(expectedDescriptionUrl == descriptionUrl,
             "The sendout description is URL is not expected description URL.");
}

void ServiceProviderShould::handle_every_search_response_from_a_device_only_once()
{
    QString discoveryResponse{"HTTP/1.1 200 OK\r\n"
                              "CACHE-CONTROL: max-age = seconds until advertisement expires\r\n"
                              "DATE: when response was generated\r\n"
                              "EXT:\r\n"
                              "LOCATION: http://127.0.0.1:8000/desc.xml\r\n"
                              "SERVER: Linux UPnP/1.0\r\n"
                              "ST: urn:schemas-upnp-org:device:MediaServer:1\r\n"
                              "USN: uuid:4d696e69-444c-164e-9d41-b827eb54e939\r\n"};

    QNetworkDatagram expectedDatagram;
    expectedDatagram.setData(discoveryResponse.toUtf8());
    expectedDatagram.setDestination(QHostAddress{"239.255.255.250"}, 1900);

    m_providerFactory->serviceDiscoveryBackendDouble->sendResponseForMediaServerRequest(expectedDatagram);
    m_providerFactory->serviceDiscoveryBackendDouble->sendResponseForMediaServerRequest(expectedDatagram);

    auto descriptionRequestSend = m_providerFactory->descriptionFetcherBackendDouble->descriptionRequestCalls;

    QVERIFY2(descriptionRequestSend == 1, "The MediaServerProvider also handles already known devices.");
}

void ServiceProviderShould::handle_sddp_bye_messages_and_inform_clients_about_the_disconnect()
{
    QSignalSpy signalSpy{m_mediaServerProvider.get(), &IServiceProvider::serviceDisconnected};
    QNetworkDatagram ssdpNotify = createServiceDiscoveryReceiveMessage(validNotifyMessage);
    QNetworkDatagram ssdpBye = createServiceDiscoveryReceiveMessage(validByeMessage);

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(ssdpNotify);
    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(ssdpBye);

    QVERIFY2(signalSpy.count() == 1, "The signal MediaServer::Disconnected must be sendout.");
}

void ServiceProviderShould::ignore_message_with_wrong_ip_and_port_notify_error()
{
    QSignalSpy signalSpy{m_mediaServerProvider.get(), &IServiceProvider::error};
    QNetworkDatagram ssdpyNotifyWrongDest = createServiceDiscoveryReceiveMessage(validNotifyMessage);
    ssdpyNotifyWrongDest.setDestination(QHostAddress{QHostAddress::LocalHost}, 33333);

    ServiceProviderError expectedError{ServiceProviderError::ErrorCode::WrongDestination,
                                       "Received package with wrong destination."};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(ssdpyNotifyWrongDest);
    QVERIFY2(signalSpy.count() == 1, "Error signal should be send.");

    auto errorObject = signalSpy.takeFirst().at(0).value<ServiceProviderError>();

    QVERIFY2(expectedError.errorCode() == errorObject.errorCode(), "");
    QVERIFY2(expectedError.errorDescription() == errorObject.errorDescription(), "");
}

void ServiceProviderShould::ignore_message_with_no_payload_and_notify_clients()
{
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::error};
    QNetworkDatagram ssdpEmptyMessage = createServiceDiscoveryReceiveMessage("");
    ssdpEmptyMessage.setSender(QHostAddress{QHostAddress::LocalHost}, 5000);

    ServiceProviderError expectedError{ServiceProviderError::ErrorCode::EmtpySSDPMessage,
                                       "Received empty ssdp message from 127.0.0.1:5000"};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(ssdpEmptyMessage);
    QVERIFY2(signalspy.count() == 1, "Error signal should be send on times");

    auto errorObject = signalspy.takeFirst().at(0).value<ServiceProviderError>();
    QVERIFY2(expectedError.errorCode() == errorObject.errorCode(), "The expected error code doesn't match");
    QVERIFY2(expectedError.errorDescription() == errorObject.errorDescription(),
             "The expected error description doesn't match");
}

void ServiceProviderShould::report_error_when_in_message_location_is_missing_and_no_byebye_type()
{
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::error};
    QString malformedSsdpPayload{"NOTIFY * HTTP/1.1\r\n"
                                 "HOST: 239.255.255.250:1900\r\n"
                                 "CACHE-CONTROL: max-age = seconds until advertisement expires \r\n"
                                 "NT: urn:schemas-upnp-org:device:MediaServer:1\r\n"
                                 "NTS: ssdp:alive\r\n"
                                 "SERVER: OS/version UPnP/1.0 product/version\r\n"
                                 "USN: uuid:4d696e69-444c-164e-9d41-b827eb54e939\r\n"};
    QNetworkDatagram malformedMessage = createServiceDiscoveryReceiveMessage(malformedSsdpPayload);
    m_providerFactory->serviceDiscoveryBackendDouble->sendResponseForMediaServerRequest(malformedMessage);

    ServiceProviderError expectedError{ServiceProviderError::ErrorCode::MalformedSsdpMessage, ""};
    QVERIFY2(signalspy.count() == 1, "The error singal should be sent.");
    auto receivedError = signalspy.takeFirst().at(0).value<ServiceProviderError>();

    QVERIFY2(expectedError.errorCode() == receivedError.errorCode(), "The expected error code doesn't match.");
}

void ServiceProviderShould::parse_device_description_without_services_no_embedded_services_and_return_values()
{
    auto expectedDeviceDescription = DeviceDescription{
        QString{"urn:schemas-upnp-org:device:MediaServer:1"},
        QString{"Test Device"},
        QString{"Blabby"},
        QString{"Test Device"},
        QString{"uuid:4d696e69-444c-164e-9d41-b827eb54e939"},
        QVector<IconDescription>{
            IconDescription{QString{"img/png"}, 64, 64, 24, QString{"http://127.0.0.1/test/img/icon_64_64.png"}},
            IconDescription{QString{"img/png"}, 32, 32, 24, QString{"http://127.0.0.1/test/img/icon_32_32.png"}}}};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceWithoutServices();

    QVERIFY2(signalspy.count() == 1, "Media server connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedDeviceDescription = m_mediaServerProvider->rootDeviceDescription(deviceUSN);

    QVERIFY2(!(expectedDeviceDescription != receivedDeviceDescription), "The unequal operator shourld return false");

    QVERIFY2(expectedDeviceDescription.friendlyName() == receivedDeviceDescription.friendlyName(),
             "Thre received friendly name is not the same.");

    QVERIFY2(expectedDeviceDescription.manufacturer() == receivedDeviceDescription.manufacturer(),
             "Thre received manufacturer name is not the same.");

    QVERIFY2(expectedDeviceDescription.modelName() == receivedDeviceDescription.modelName(),
             "Thre received model name is not the same.");

    QVERIFY2(expectedDeviceDescription.udn() == receivedDeviceDescription.udn(),
             "Thre received udn name is not the same.");

    QVERIFY2(expectedDeviceDescription.icons().size() == receivedDeviceDescription.icons().size(),
             "The icon list has not the expected size.");

    for (auto index = 0; index < receivedDeviceDescription.icons().size(); ++index)
    {
        QVERIFY2(expectedDeviceDescription.icons().at(index).mimeType() ==
                     receivedDeviceDescription.icons().at(index).mimeType(),
                 QString{"The mimetype of icon %1 is not the same"}.arg(index).toUtf8().data());

        QVERIFY2(expectedDeviceDescription.icons().at(index).width() ==
                     receivedDeviceDescription.icons().at(index).width(),
                 QString{"The width of icon %1 is not the same"}.arg(index).toUtf8().data());

        QVERIFY2(expectedDeviceDescription.icons().at(index).height() ==
                     receivedDeviceDescription.icons().at(index).height(),
                 QString{"The height of icon %1 is not the same"}.arg(index).toUtf8().data());

        QVERIFY2(expectedDeviceDescription.icons().at(index).depth() ==
                     receivedDeviceDescription.icons().at(index).depth(),
                 QString{"The depth of icon %1 is not the same"}.arg(index).toUtf8().data());

        QVERIFY2(expectedDeviceDescription.icons().at(index).url() == receivedDeviceDescription.icons().at(index).url(),
                 QString{"The depth of icon %1 is not the same"}.arg(index).toUtf8().data());
    }

    QVERIFY2(expectedDeviceDescription == receivedDeviceDescription,
             "Thre received phyiscal device description is not the same.");

    QVERIFY2(!(expectedDeviceDescription != receivedDeviceDescription), "The unequal operator shourld return false");
}

void ServiceProviderShould::parse_device_description_with_embedded_devices_without_services()
{
    auto expectedDeviceDescription = DeviceDescription{
        QString{"urn:schemas-upnp-org:device:MediaServer:1"},
        QString{"Test Device"},
        QString{"Blabby"},
        QString{"Test Device"},
        QString{"uuid:4d696e69-444c-164e-9d41-b827eb54e959"},
        QVector<IconDescription>{
            IconDescription{QString{"img/png"}, 64, 64, 24, QString{"http://127.0.0.1/test/img/icon_64_64.png"}},
            IconDescription{QString{"img/png"}, 32, 32, 24, QString{"http://127.0.0.1/test/img/icon_32_32.png"}}}};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceWithEmbeddedDevicesAndWithoutServices();

    QVERIFY2(signalspy.count() == 1, "Media server connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedDeviceDescription = m_mediaServerProvider->rootDeviceDescription(deviceUSN);

    QVERIFY2(expectedDeviceDescription == receivedDeviceDescription,
             "The received phyiscal device description is not the expected.");
}

void ServiceProviderShould::parse_device_description_with_empty_base_url()
{
    auto expectedPhysicalDeviceDescription = DeviceDescription{
        QString{"urn:schemas-upnp-org:device:MediaServer:1"},
        QString{"Test Device"},
        QString{"Blabby"},
        QString{"Test Device"},
        QString{"uuid:4d696e69-444c-164e-9d41-b827eb54e939"},
        QVector<IconDescription>{
            IconDescription{QString{"img/png"}, 64, 64, 24, QString{"http://127.0.0.1:8000/img/icon_64_64.png"}},
            IconDescription{QString{"img/png"}, 32, 32, 24, QString{"http://127.0.0.1:8000/img/icon_32_32.png"}}}};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceDescriptionEmptyBaseUrl();

    QVERIFY2(signalspy.count() == 1, "Media server connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedDeviceDescription = m_mediaServerProvider->rootDeviceDescription(deviceUSN);

    QVERIFY2(expectedPhysicalDeviceDescription == receivedDeviceDescription,
             "Thre received phyiscal device description is not the same.");
}

void ServiceProviderShould::parse_device_description_base_url_after_device_description()
{
    auto expectedDeviceDescription = DeviceDescription{
        QString{"urn:schemas-upnp-org:device:MediaServer:1"},
        QString{"Test Device"},
        QString{"Blabby"},
        QString{"Test Device"},
        QString{"uuid:4d696e69-444c-164e-9d41-b827eb54e939"},
        QVector<IconDescription>{
            IconDescription{QString{"img/png"}, 64, 64, 24, QString{"http://127.0.0.1/test/img/icon_64_64.png"}},
            IconDescription{QString{"img/png"}, 32, 32, 24, QString{"http://127.0.0.1/test/img/icon_32_32.png"}}}};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceDescriptionBaseUrlAfterDeviceDefinition();

    QVERIFY2(signalspy.count() == 1, "Service connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedDeviceDescription = m_mediaServerProvider->rootDeviceDescription(deviceUSN);

    QVERIFY2(expectedDeviceDescription == receivedDeviceDescription,
             "Thre received phyiscal device description is not the same.");
}

void ServiceProviderShould::parse_devices_description_with_services()
{
    auto expectedDeviceDescription = DeviceDescription{
        QString{"urn:schemas-upnp-org:device:MediaServer:1"},
        QString{"Test Device"},
        QString{"Blabby"},
        QString{"Test Device"},
        QString{"uuid:4d696e69-444c-164e-9d41-b827eb54e959"},
        QVector<IconDescription>{
            IconDescription{QString{"img/png"}, 64, 64, 24, QString{"http://127.0.0.1/test/img/icon_64_64.png"}},
            IconDescription{QString{"img/png"}, 32, 32, 24, QString{"http://127.0.0.1/test/img/icon_32_32.png"}}},
        QVector<ServiceDescription>{ServiceDescription{QString{"urn:schemas-upnp-org:service:SERVICE:1"},
                                                       QString{"urn:upnp-org:serviceId:SerivceId5"},
                                                       QString{"http://127.0.0.1/test/serviceId1.xml"},
                                                       QString{"http://127.0.0.1/test/ctl/serviceId1"},
                                                       QString{"http://127.0.0.1/test/evt/serviceId1"}}}};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceWithServicesAndEmbeddedDevicesWithServices();

    QVERIFY2(signalspy.count() == 1, "Media server connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedDeviceDescription = m_mediaServerProvider->rootDeviceDescription(deviceUSN);

    QVERIFY2(expectedDeviceDescription == receivedDeviceDescription,
             "The received number of devices is not the expected.");

    QVERIFY2(expectedDeviceDescription.services().size() == receivedDeviceDescription.services().size(),
             QString{"Expected: %1 : Actual: %2"}
                 .arg(expectedDeviceDescription.services().size())
                 .arg(receivedDeviceDescription.services().size())
                 .toUtf8()
                 .data());

    for (auto serviceIndex = 0; serviceIndex < receivedDeviceDescription.services().size(); ++serviceIndex)
    {
        auto receivedServiceDescription = receivedDeviceDescription.services().at(serviceIndex);
        auto expectedServiceDescription = expectedDeviceDescription.services().at(serviceIndex);

        QVERIFY2(expectedServiceDescription.serviceType() == receivedServiceDescription.serviceType(),
                 QString{"Expected: %1 : Received: %2"}
                     .arg(expectedServiceDescription.serviceType())
                     .arg(receivedServiceDescription.serviceType())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedServiceDescription.id() == receivedServiceDescription.id(),
                 QString{"Expected: %1 : Received: %2"}
                     .arg(expectedServiceDescription.id())
                     .arg(receivedServiceDescription.id())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedServiceDescription.scpdUrl() == receivedServiceDescription.scpdUrl(),
                 QString{"Expected: %1 : Received: %2"}
                     .arg(expectedServiceDescription.id())
                     .arg(receivedServiceDescription.id())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedServiceDescription.controlUrl() == receivedServiceDescription.controlUrl(),
                 QString{"Expected: %1 : Received: %2"}
                     .arg(expectedServiceDescription.controlUrl())
                     .arg(receivedServiceDescription.controlUrl())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedServiceDescription.eventUrl() == receivedServiceDescription.eventUrl(),
                 QString{"Expected: %1 : Received: %2"}
                     .arg(expectedServiceDescription.eventUrl())
                     .arg(receivedServiceDescription.eventUrl())
                     .toUtf8()
                     .data());

        QVERIFY(expectedServiceDescription == receivedServiceDescription);
        QVERIFY(!(expectedServiceDescription != receivedServiceDescription));
    }
}

void ServiceProviderShould::ignore_broken_device_description_notify_error()
{
    QSignalSpy signalSpy(m_mediaServerProvider.get(), &IServiceProvider::error);
    ServiceProviderError expectedError{
        ServiceProviderError::ErrorCode::XmlNotWellFormed,
        QStringLiteral("Received XML from http://127.0.0.1:8000/desc.xml is not well formed")};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendDeviceXmlNotWellFormed();

    QVERIFY2(signalSpy.count() == 1, "Error signal should be emitted");
    auto error = signalSpy.takeFirst().at(0).value<ServiceProviderError>();

    QVERIFY2(expectedError.errorCode() == error.errorCode(),
             QString{"Expected: %1 : Actual:%2"}
                 .arg(static_cast<qint32>(expectedError.errorCode()))
                 .arg(static_cast<qint32>(error.errorCode()))
                 .toUtf8()
                 .data());

    QVERIFY2(expectedError.errorDescription() == error.errorDescription(),
             QString{"Expected: %1 : Actual:%2"}
                 .arg(expectedError.errorDescription())
                 .arg(error.errorDescription())
                 .toUtf8()
                 .data());
}

void ServiceProviderShould::request_scpd_configurations()
{
    QUrl expectedUrl{"http://127.0.0.1/test/serviceId1.xml"};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendOneDeviceOneService();

    // expect to calls one to device description and one to the scpd.
    QVERIFY2(2 == m_providerFactory->descriptionFetcherBackendDouble->descriptionRequestCalls,
             QString{"Expected 1 : Actual %1"}
                 .arg(m_providerFactory->descriptionFetcherBackendDouble->descriptionRequestCalls)
                 .toUtf8()
                 .data());

    QVERIFY2(expectedUrl == m_providerFactory->descriptionFetcherBackendDouble->lastDescriptionRequest,
             QString{"Expected %1 : Actual %2"}
                 .arg(expectedUrl.toString())
                 .arg(m_providerFactory->descriptionFetcherBackendDouble->lastDescriptionRequest.toString())
                 .toUtf8()
                 .data());
}

void ServiceProviderShould::parse_service_control_point_definition()
{
    auto scpdStateVariables = QVector<SCPDStateVariable>{
        SCPDStateVariable{false, QString{"Value1"}, SCPDStateVariable::DataType::Ui1, QString{"0"}},

        SCPDStateVariable{false,
                          QString{"Value2"},
                          SCPDStateVariable::DataType::Ui2,
                          QString{"1"},
                          QVector<QString>{QString{"2"}, QString{"3"}}},

        SCPDStateVariable{
            false,
            QString{"Value3"},
            SCPDStateVariable::DataType::Ui4,
            QString{"2"},
            QVector<QString>{},
            QString{"0"},
            QString{"2"},
            QString{"1"},
        },

        SCPDStateVariable{
            false,
            QString{"Value4"},
            SCPDStateVariable::DataType::I1,
            QString{"0"},
        },

        SCPDStateVariable{
            false,
            QString{"Value5"},
            SCPDStateVariable::DataType::I2,
            QString{"0"},
        },

        SCPDStateVariable{
            false,
            QString{"Value6"},
            SCPDStateVariable::DataType::I4,
            QString{"0"},
        },

        SCPDStateVariable{
            false,
            QString{"Value7"},
            SCPDStateVariable::DataType::R4,
            QString{"0"},
        },

        SCPDStateVariable{
            false,
            QString{"Value8"},
            SCPDStateVariable::DataType::R8,
            QString{"0"},
        },

        SCPDStateVariable{
            false,
            QString{"Value9"},
            SCPDStateVariable::DataType::Char,
            QString{"a"},
        },

        SCPDStateVariable{
            false,
            QString{"Value10"},
            SCPDStateVariable::DataType::String,
            QString{"hallowelt"},
        },

        SCPDStateVariable{
            false,
            QString{"Value11"},
            SCPDStateVariable::DataType::Date,
        },

        SCPDStateVariable{
            false,
            QString{"Value12"},
            SCPDStateVariable::DataType::DateTime,
        },

        SCPDStateVariable{
            false,
            QString{"Value13"},
            SCPDStateVariable::DataType::DateTimeTz,
        },

        SCPDStateVariable{
            false,
            QString{"Value14"},
            SCPDStateVariable::DataType::Time,
        },

        SCPDStateVariable{
            false,
            QString{"Value15"},
            SCPDStateVariable::DataType::TimeTz,
        },

        SCPDStateVariable{false, QString{"Value16"}, SCPDStateVariable::DataType::Bool, QString{"0"}},

        SCPDStateVariable{
            false,
            QString{"Value17"},
            SCPDStateVariable::DataType::Uri,
        },

        SCPDStateVariable{
            false,
            QString{"Value18"},
            SCPDStateVariable::DataType::Uuid,
        },
    };

    auto scpdActions = QVector<SCPDAction>{
        SCPDAction{QString{"SetValue"},
                   QVector<SCPDArgument>{SCPDArgument{QString{"NewValue"}, SCPDArgument::In, QString{"Value1"}}}},

        SCPDAction{
            QString{"SetValue2"},
            QVector<SCPDArgument>{SCPDArgument{QString{"NewValue"}, SCPDArgument::In, QString{"Value1"}},

                                  SCPDArgument{QString{"NewValue2"}, SCPDArgument::In, QString{"Value2"}}},
        },

        SCPDAction{QString{"GetValue"},
                   QVector<SCPDArgument>{SCPDArgument{QString{"Value"}, SCPDArgument::Out, QString{"Value1"}}}},

        SCPDAction{
            QString{"GetValue2"},
            QVector<SCPDArgument>{SCPDArgument{QString{"Value"}, SCPDArgument::Out, QString{"Value1"}},

                                  SCPDArgument{QString{"Value2"}, SCPDArgument::Out, QString{"Value2"}}},
        },
    };

    auto expectedScpd =
        ServiceControlPointDefinition{QString{"http://127.0.0.1/test/serviceId1.xml"}, scpdStateVariables, scpdActions};
    QSignalSpy signalspy{m_mediaServerProvider.get(), &IServiceProvider::serviceConnected};

    m_providerFactory->serviceDiscoveryBackendDouble->sendNotifyMessage(
        createServiceDiscoveryReceiveMessage(validNotifyMessage));
    m_providerFactory->descriptionFetcherBackendDouble->sendOneDeviceOneService();

    QVERIFY2(signalspy.count() == 1, "Service connected signal should be emitted.");
    auto deviceUSN = signalspy.takeFirst().at(0).value<QString>();
    auto receivedScpd = m_mediaServerProvider->rootDeviceDescription(deviceUSN).scpds();

    QVERIFY2(receivedScpd.size() == 1, QString{"Expected: 1 : Actual %2"}.arg(receivedScpd.size()).toUtf8().data());

    QVERIFY2(expectedScpd.serviceStateTable().size() == receivedScpd.at(0).serviceStateTable().size(),
             QString{"Expected: %1 : Actual %2"}
                 .arg(expectedScpd.serviceStateTable().size())
                 .arg(receivedScpd.at(0).serviceStateTable().size())
                 .toUtf8()
                 .data());

    auto stateVariableIndex = 0;
    for (; stateVariableIndex < receivedScpd.at(0).serviceStateTable().size(); ++stateVariableIndex)
    {
        auto stateVariable = receivedScpd.at(0).serviceStateTable().at(stateVariableIndex);
        auto expectedStateVariable = expectedScpd.serviceStateTable().at(stateVariableIndex);

        QVERIFY2(expectedStateVariable.name() == stateVariable.name(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.name())
                     .arg(stateVariable.name())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.dataType() == stateVariable.dataType(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.dataType())
                     .arg(stateVariable.dataType())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.defaultValue() == stateVariable.defaultValue(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.defaultValue())
                     .arg(stateVariable.defaultValue())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.allowedValues() == stateVariable.allowedValues(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.allowedValues().size())
                     .arg(stateVariable.allowedValues().size())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.miniumValue() == stateVariable.miniumValue(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.miniumValue())
                     .arg(stateVariable.miniumValue())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.maximumValue() == stateVariable.maximumValue(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.name())
                     .arg(stateVariable.name())
                     .toUtf8()
                     .data());

        QVERIFY2(expectedStateVariable.maximumValue() == stateVariable.maximumValue(),
                 QString{"Expected: %1 : Actual %2"}
                     .arg(expectedStateVariable.name())
                     .arg(stateVariable.name())
                     .toUtf8()
                     .data());
    }

    QVERIFY2(expectedScpd.actionList().size() == receivedScpd.at(0).actionList().size(),
             QString{"Expected: %1 : Actual %2"}
                 .arg(expectedScpd.actionList().size())
                 .arg(receivedScpd.at(0).actionList().size())
                 .toUtf8()
                 .data());

    auto actionIndex = 0;
    for (; actionIndex < receivedScpd.at(0).actionList().size(); ++actionIndex)
    {
        auto action = receivedScpd.at(0).actionList().at(actionIndex);
        auto expectedAction = expectedScpd.actionList().at(actionIndex);

        QVERIFY2(expectedAction.name() == action.name(),
                 QString{"Expected: %1 : Actual %2"}.arg(expectedAction.name()).arg(action.name()).toUtf8().data());

        auto actionArgs = action.arguments();
        auto expectedActionArgs = expectedAction.arguments();

        QVERIFY2(
            expectedActionArgs.size() == actionArgs.size(),
            QString{"Expected: %1 : Actual %2"}.arg(expectedActionArgs.size()).arg(actionArgs.size()).toUtf8().data());

        auto argIndex = 0;
        for (; argIndex < actionArgs.size(); ++argIndex)
        {
            QVERIFY2(expectedActionArgs.at(argIndex).name() == actionArgs.at(argIndex).name(),
                     QString{"Expected %1 : Actual %2"}
                         .arg(expectedActionArgs.at(argIndex).name())
                         .arg(actionArgs.at(argIndex).name())
                         .toUtf8()
                         .data());

            QVERIFY2(expectedActionArgs.at(argIndex).direction() == actionArgs.at(argIndex).direction(),
                     QString{"Expected %1 : Actual %2"}
                         .arg(expectedActionArgs.at(argIndex).direction())
                         .arg(actionArgs.at(argIndex).direction())
                         .toUtf8()
                         .data());

            QVERIFY2(expectedActionArgs.at(argIndex).relatedStateVariable() ==
                         actionArgs.at(argIndex).relatedStateVariable(),
                     QString{"Expected %1 : Actual %2"}
                         .arg(expectedActionArgs.at(argIndex).relatedStateVariable())
                         .arg(actionArgs.at(argIndex).relatedStateVariable())
                         .toUtf8()
                         .data());
        }
    }

    // Test implemented operators
    QVERIFY2(expectedScpd == receivedScpd.at(0), "The equal operator returns wrong value.");

    QVERIFY2(!(expectedScpd != receivedScpd.at(0)), "The unequal operator returns wrong value");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::ServiceProviderShould)
