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

#include "RootDeviceDescription.h"

#include <QSharedPointer>
#include <QObject>

class QNetworkDatagram;

namespace UPnPAV
{
class ServiceDiscovery;
class ServiceDiscoveryBackend;
class DescriptionFetcher;
class DescriptionFetcherBackend;
class DeviceDescriptionParser;
class ServiceDiscoveryPackage;
class ServiceProviderError;

/**
 *
 */
class ServiceProvider : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProvider)
public:
    ServiceProvider(const QString searchTarget,
                    const QSharedPointer<ServiceDiscoveryBackend> &serviceDiscoverybackend,
                    const QSharedPointer<DescriptionFetcherBackend> &descriptionFetcherBackend);
    ~ServiceProvider() override;

    void startSearch();

    DeviceDescription rootDeviceDescription(const QString &usn);

Q_SIGNALS:
    void serviceConnected(const QString &uniqueServiceName);
    void serviceDisconnected(const QString &uniqueServiceName);
    void error(const UPnPAV::ServiceProviderError &errorObject);

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

    QSharedPointer<ServiceDiscovery> m_serviceDiscovery;
    QSharedPointer<DescriptionFetcher> m_descriptionFetcher;

    QVector<QString> m_knownDevices;
    QVector<QUrl> m_pendingDeviceDescription;

    QHash<QString, TempDeviceDescription> m_tempDeviceDescriptions;
    QHash<QString, DeviceDescription> m_deviceDescriptions;
};

class ServiceProviderFactory
{
public:
    virtual ~ServiceProviderFactory();
    virtual QSharedPointer<ServiceProvider> createServiceProvider();
};

class ServiceProviderError
{
public:
    enum class ErrorCode : quint8
    {
        WrongDestination = 0, //!< The SSDP message
        EmtpySSDPMessage,
        MalformedSsdpMessage,
        XmlNotWellFormed
    };

    ServiceProviderError();
    ServiceProviderError(ErrorCode errorCode,
                         const QString &errorDescription) noexcept;

    ~ServiceProviderError() noexcept;

    ServiceProviderError(const ServiceProviderError &other);
    ServiceProviderError& operator=(const ServiceProviderError &other);

    ErrorCode errorCode() const;

    QString errorDescription() const;

private:
    ErrorCode m_errorCode;
    QString m_errorDescription;
};

} //UPnPAV

Q_DECLARE_METATYPE(UPnPAV::ServiceProviderError)

#endif // SERVICEPROVIDER_H
