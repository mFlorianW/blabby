// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include "IServiceProvider.hpp"
#include "blabbyupnpav_export.h"
#include <QHash>
#include <QObject>
#include <memory>

class QNetworkDatagram;

namespace UPnPAV
{
class ServiceDiscovery;
class ServiceDiscoveryBackend;
class DescriptionFetcher;
class DescriptionFetcherBackend;
class DeviceDescriptionParser;
class ServiceDiscoveryPackage;

class BLABBYUPNPAV_EXPORT ServiceProvider : public IServiceProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProvider)
public:
    ServiceProvider();
    ServiceProvider(const QString searchTarget,
                    std::unique_ptr<ServiceDiscoveryBackend> serviceDiscoverybackend,
                    std::unique_ptr<DescriptionFetcherBackend> descriptionFetcherBackend);
    ~ServiceProvider() override;

    void setSearchTarget(const QString &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    DeviceDescription rootDeviceDescription(const QString &usn) const noexcept override;

private Q_SLOTS:
    void handleServiceDiscoveryMessage(const QNetworkDatagram &datagram);

    void handleFetchedDescription(const QString &description, const QUrl &url);
    void handleFetchedDeviceDescription(const QString &deviceDescription, const QUrl &url);
    void handleFetchSCPDDescription(const QString &scpdDescription, const QUrl &url);

private:
    void handleByeByePackage(const ServiceDiscoveryPackage &package);
    void handlePackage(const ServiceDiscoveryPackage &package);
    void handleparsedDeviceDescription(const DeviceDescription &deviceDescription);

    static bool validateDestination(const QNetworkDatagram &datagram);

private:
    struct TempDeviceDescription
    {
        QString udn;
        DeviceDescription deviceDescriptions;
        QVector<ServiceControlPointDefinition> scpds;
        QVector<QString> pendingSCPDS;
    };

    QString m_searchTarget{""};

    std::unique_ptr<ServiceDiscoveryBackend> mServiceDiscoveryBackend;
    std::unique_ptr<ServiceDiscovery> m_serviceDiscovery;
    std::unique_ptr<DescriptionFetcherBackend> mDescriptionFetcherBackend;
    std::unique_ptr<DescriptionFetcher> m_descriptionFetcher;

    QVector<QString> m_knownDevices;
    QVector<QUrl> m_pendingDeviceDescription;

    QHash<QString, TempDeviceDescription> m_tempDeviceDescriptions;
    QHash<QString, DeviceDescription> m_deviceDescriptions;
};

class BLABBYUPNPAV_EXPORT ServiceProviderFactory : public IServiceProviderFactory
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderFactory)
public:
    ServiceProviderFactory();
    ~ServiceProviderFactory() override;
    std::unique_ptr<IServiceProvider> createServiceProvider(const QString &searchTarget) override;
};

} // namespace UPnPAV

#endif // SERVICEPROVIDER_H
