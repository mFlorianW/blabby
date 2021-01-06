/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include "IServiceProvider.h"
#include "UPnP_Export.h"
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


class UPNP_EXPORT ServiceProvider : public IServiceProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProvider)
public:
    ServiceProvider();
    ServiceProvider(const QString searchTarget, std::unique_ptr<ServiceDiscoveryBackend> serviceDiscoverybackend,
                    std::unique_ptr<DescriptionFetcherBackend> descriptionFetcherBackend);
    ~ServiceProvider() override;

    void setSearchTarget(const QString &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    DeviceDescription rootDeviceDescription(const QString &usn) const noexcept;

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

    QString m_searchTarget{ "" };

    std::unique_ptr<ServiceDiscoveryBackend> mServiceDiscoveryBackend;
    std::unique_ptr<ServiceDiscovery> m_serviceDiscovery;
    std::unique_ptr<DescriptionFetcherBackend> mDescriptionFetcherBackend;
    std::unique_ptr<DescriptionFetcher> m_descriptionFetcher;

    QVector<QString> m_knownDevices;
    QVector<QUrl> m_pendingDeviceDescription;

    QHash<QString, TempDeviceDescription> m_tempDeviceDescriptions;
    QHash<QString, DeviceDescription> m_deviceDescriptions;
};

class UPNP_EXPORT ServiceProviderFactory : public IServiceProviderFactory
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderFactory)
public:
    ServiceProviderFactory();
    virtual ~ServiceProviderFactory();
    virtual std::unique_ptr<IServiceProvider> createServiceProvider(const QString &searchTarget);
};

} // namespace UPnPAV

#endif // SERVICEPROVIDER_H
