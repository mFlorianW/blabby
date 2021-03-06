// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceProvider.h"
#include "DescriptionFetcher.h"
#include "DescriptionFetcherBackend.h"
#include "DeviceDescriptionParser.h"
#include "ParsingError.h"
#include "ServiceControlPointDefinitionParser.h"
#include "ServiceDiscovery.h"
#include "ServiceDiscoveryBackend.h"
#include "ServiceDiscoveryPackage.h"
#include "ServiceProviderError.h"

#include <QNetworkDatagram>
#include <QUrl>

namespace
{
auto registerMediaServerProviderError = qRegisterMetaType<UPnPAV::ServiceProviderError>();
}

namespace UPnPAV
{

ServiceProvider::ServiceProvider() = default;

ServiceProvider::ServiceProvider(const QString searchTarget, std::unique_ptr<ServiceDiscoveryBackend> serviceDiscoverybackend,
                                 std::unique_ptr<DescriptionFetcherBackend> descriptionFetcherBackend)
    : IServiceProvider()
    , m_searchTarget(searchTarget)
    , mServiceDiscoveryBackend(std::move(serviceDiscoverybackend))
    , m_serviceDiscovery(std::make_unique<ServiceDiscovery>(mServiceDiscoveryBackend.get()))
    , mDescriptionFetcherBackend(std::move(descriptionFetcherBackend))
    , m_descriptionFetcher(std::make_unique<DescriptionFetcher>(mDescriptionFetcherBackend.get()))
{
    (void)connect(m_serviceDiscovery.get(), &ServiceDiscovery::dataReceived, this, &ServiceProvider::handleServiceDiscoveryMessage);
    (void)connect(m_descriptionFetcher.get(), &DescriptionFetcher::descriptionFetched, this, &ServiceProvider::handleFetchedDescription);
}

ServiceProvider::~ServiceProvider() = default;

void ServiceProvider::setSearchTarget(const QString &searchTarget) noexcept
{
    m_searchTarget = searchTarget;
}

void ServiceProvider::startSearch() const noexcept
{
    m_serviceDiscovery->sendSearchRequest(m_searchTarget);
}

DeviceDescription ServiceProvider::rootDeviceDescription(const QString &usn) const noexcept
{
    return m_deviceDescriptions.value(usn);
}

bool ServiceProvider::validateDestination(const QNetworkDatagram &datagram)
{
    return (datagram.destinationAddress() == QHostAddress{ "239.255.255.250" }) && (datagram.destinationPort() == 1900);
}

void ServiceProvider::handleByeByePackage(const ServiceDiscoveryPackage &package)
{
    m_knownDevices.removeAll(package.deviceId());
    Q_EMIT serviceDisconnected(package.deviceId());
}

void ServiceProvider::handlePackage(const ServiceDiscoveryPackage &package)
{
    // We already know the device we can ignore the message.
    if(m_knownDevices.contains(package.deviceId()))
    {
        return;
    }

    m_descriptionFetcher->fetchDescription(package.locationUrl());
    m_knownDevices.push_back(package.deviceId());
    m_pendingDeviceDescription.append(package.locationUrl());
}

void ServiceProvider::handleServiceDiscoveryMessage(const QNetworkDatagram &datagram)
{
    if(datagram.data().isEmpty())
    {
        QString errorMessage =
            QString{ "Received empty ssdp message from %1:%2" }.arg(datagram.senderAddress().toString()).arg(datagram.senderPort());

        Q_EMIT error(ServiceProviderError{ ServiceProviderError::ErrorCode::EmtpySSDPMessage, errorMessage });
        return;
    }

    try
    {
        ServiceDiscoveryPackage package{ datagram.data() };

        if((package.notificationSubType() == ServiceDiscoveryPackage::ByeBye) && m_knownDevices.contains(package.deviceId()))
        {
            handleByeByePackage(package);
            return;
        }

        if((package.notificationSubType() == ServiceDiscoveryPackage::Notify) && (!validateDestination(datagram)))
        {
            Q_EMIT error(ServiceProviderError{ ServiceProviderError::ErrorCode::WrongDestination,
                                               "Received package with wrong destination." });
            return;
        }

        handlePackage(package);
    }
    catch(const PackageParseError &e)
    {
        // TODO: Use category logging for details
        QString errorMessage =
            QString{ "Received malformd SSDP message from %1:%2" }.arg(datagram.senderAddress().toString()).arg(datagram.senderPort());

        Q_EMIT error(ServiceProviderError{ ServiceProviderError::ErrorCode::MalformedSsdpMessage, errorMessage });
    }
}

void ServiceProvider::handleFetchedDescription(const QString &description, const QUrl &url)
{
    if(m_pendingDeviceDescription.contains(url))
    {
        handleFetchedDeviceDescription(description, url);
    }
    else
    {
        handleFetchSCPDDescription(description, url);
    }
}

void ServiceProvider::handleparsedDeviceDescription(const DeviceDescription &deviceDescription)
{
    if(deviceDescription.services().isEmpty())
    {
        m_deviceDescriptions.insert(deviceDescription.udn(), deviceDescription);
        Q_EMIT serviceConnected(deviceDescription.udn());
    }
    else
    {
        // request all scpds
        auto tempDeviceDescription = TempDeviceDescription{};
        for(const auto &serviceDescription : deviceDescription.services())
        {
            tempDeviceDescription.udn = deviceDescription.udn();
            tempDeviceDescription.deviceDescriptions = deviceDescription;
            tempDeviceDescription.pendingSCPDS.append(serviceDescription.scpdUrl());
            m_tempDeviceDescriptions.insert(deviceDescription.udn(), tempDeviceDescription);

            m_descriptionFetcher->fetchDescription(serviceDescription.scpdUrl());
        }
    }
}

void ServiceProvider::handleFetchedDeviceDescription(const QString &deviceDescription, const QUrl &url)
{
    m_pendingDeviceDescription.removeAll(url);

    auto deviceReader = DeviceDescriptionParser(url);
    try
    {
        deviceReader.readDeviceDescription(deviceDescription);
    }
    catch(const ParsingError &e)
    {
        Q_EMIT error(ServiceProviderError{ ServiceProviderError::ErrorCode::XmlNotWellFormed,
                                           QString("Received XML from %1 is not well formed").arg(url.toString()) });
        return;
    }

    auto devices = deviceReader.physicalDeviceDescription();
    for(const auto &parsedDeviceDescription : devices)
    {
        if(parsedDeviceDescription.deviceType() == m_searchTarget)
        {
            handleparsedDeviceDescription(parsedDeviceDescription);
        }
    }
}

void ServiceProvider::handleFetchSCPDDescription(const QString &scpdDescription, const QUrl &url)
{
    // TODO: Maybe replace with std::find_if
    auto foundIter = m_tempDeviceDescriptions.end();
    for(auto iter = m_tempDeviceDescriptions.begin(); iter != m_tempDeviceDescriptions.end(); ++iter)
    {
        if(iter.value().pendingSCPDS.contains(url.toString()))
        {
            foundIter = iter;
            break;
        }
    }

    if(foundIter == m_tempDeviceDescriptions.end())
    {
        return;
    }

    ServiceControlPointDefinitionParser scpdParser{ url.toString() };
    try
    {
        scpdParser.parseServiceControlPointDefinition(scpdDescription);
    }
    catch(const ParsingError &e)
    {
        Q_EMIT error(ServiceProviderError{ ServiceProviderError::ErrorCode::XmlNotWellFormed,
                                           QString("Received XML from %1 is not well formed").arg(url.toString()) });
        return;
    }

    foundIter.value().pendingSCPDS.removeAll(url.toString());
    foundIter.value().scpds.append(scpdParser.serviceControlPointDefinition());
    if(foundIter.value().pendingSCPDS.isEmpty())
    {
        DeviceDescription deviceDescription{
            foundIter.value().deviceDescriptions.deviceType(),   foundIter.value().deviceDescriptions.friendlyName(),
            foundIter.value().deviceDescriptions.manufacturer(), foundIter.value().deviceDescriptions.modelName(),
            foundIter.value().deviceDescriptions.udn(),          foundIter.value().deviceDescriptions.icons(),
            foundIter.value().deviceDescriptions.services(),     foundIter.value().scpds
        };

        m_deviceDescriptions.insert(deviceDescription.udn(), deviceDescription);
        Q_EMIT serviceConnected(deviceDescription.udn());
    }
}

ServiceProviderError::ServiceProviderError()
{
}

ServiceProviderError::ServiceProviderError(ErrorCode errorCode, const QString &errorDescription) noexcept
    : m_errorCode(errorCode)
    , m_errorDescription(errorDescription)
{
}

ServiceProviderError::~ServiceProviderError() noexcept
{
}


ServiceProviderFactory::ServiceProviderFactory() = default;
ServiceProviderFactory::~ServiceProviderFactory() = default;

std::unique_ptr<IServiceProvider> ServiceProviderFactory::createServiceProvider(const QString &searchTarget)
{
    auto serviceDiscoveryBackend = std::make_unique<UdpServiceDiscoveryBackend>();
    auto descriptionFetcherBackend = std::make_unique<HttpDescriptionFetcherBackend>();

    return std::make_unique<ServiceProvider>(searchTarget, std::move(serviceDiscoveryBackend), std::move(descriptionFetcherBackend));
}

} // namespace UPnPAV
