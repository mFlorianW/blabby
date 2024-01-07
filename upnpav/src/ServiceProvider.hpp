// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
                    std::shared_ptr<ServiceDiscoveryBackend> serviceDiscoverybackend,
                    std::unique_ptr<DescriptionFetcherBackend> descriptionFetcherBackend);
    ~ServiceProvider() override;

    void setSearchTarget(QString const &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    DeviceDescription rootDeviceDescription(QString const &usn) const noexcept override;

private Q_SLOTS:
    void handleServiceDiscoveryMessage(QNetworkDatagram const &datagram);

    void handleFetchedDescription(QString const &description, QUrl const &url);
    void handleFetchedDeviceDescription(QString const &deviceDescription, QUrl const &url);
    void handleFetchSCPDDescription(QString const &scpdDescription, QUrl const &url);

private:
    void handleByeByePackage(ServiceDiscoveryPackage const &package);
    void handlePackage(ServiceDiscoveryPackage const &package);
    void handleparsedDeviceDescription(DeviceDescription const &deviceDescription);

    static bool validateDestination(QNetworkDatagram const &datagram);

private:
    struct TempDeviceDescription
    {
        QString udn;
        DeviceDescription deviceDescriptions;
        QVector<ServiceControlPointDefinition> scpds;
        QVector<QString> pendingSCPDS;
    };

    QString m_searchTarget{""};

    std::shared_ptr<ServiceDiscoveryBackend> mServiceDiscoveryBackend;
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
    std::unique_ptr<IServiceProvider> createServiceProvider(QString const &searchTarget) override;
};

} // namespace UPnPAV

#endif // SERVICEPROVIDER_H
