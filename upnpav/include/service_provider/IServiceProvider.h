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
#ifndef ISERVICEPROVIDER_H
#define ISERVICEPROVIDER_H

#include "DeviceDescription.h"
#include "ServiceProviderError.h"
#include "UPnP_Export.h"
#include <QObject>

namespace UPnPAV
{

/**
 * The IServiceProvider defines the interface for a UPnP service provider.
 */
class UPNP_EXPORT IServiceProvider : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(IServiceProvider)
public:
    /**
     * Destroy the IServiceProvider object.
     */
    virtual ~IServiceProvider() noexcept = default;

    /**
     * Set the search for a service.
     * @param setSearchTarget The search target of the service.
     */
    virtual void setSearchTarget(const QString &searchTarget) noexcept = 0;

    /**
     * Starts a search.
     */
    virtual void startSearch() const noexcept = 0;

    /**
     * Returns the DeviceDescription for a unique server name.
     *
     * @param usn The unique service name
     * @return DeviceDescription The device description.
     */
    virtual DeviceDescription rootDeviceDescription(const QString &usn) const noexcept = 0;

Q_SIGNALS:
    /**
     * This signal is emitted when a service for the specified search target is connected to the network.
     * @param uniqueServiceName The service name which is connected.
     */
    void serviceConnected(const QString &uniqueServiceName);

    /**
     * This signal is emitted when a service is disconnected form the network.
     * @param uniqueServiceName
     */
    void serviceDisconnected(const QString &uniqueServiceName);

    /**
     * This signal is emitted when an error occur during the service discovery.
     * @param errorObject The error object that contains a specific error message.
     */
    void error(const UPnPAV::ServiceProviderError &errorObject);

protected:
    IServiceProvider() = default;
};

} // namespace UPnPAV

#endif // ISERVICEPROVIDER_H