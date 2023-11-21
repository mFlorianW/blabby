// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef ISERVICEPROVIDER_H
#define ISERVICEPROVIDER_H

#include "DeviceDescription.hpp"
#include "ServiceProviderError.hpp"
#include "blabbyupnpav_export.h"
#include <QObject>
#include <memory>

namespace UPnPAV
{

/**
 * The IServiceProvider defines the interface for a UPnP service provider.
 */
class BLABBYUPNPAV_EXPORT IServiceProvider : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(IServiceProvider)
public:
    /**
     * Destroy the IServiceProvider object.
     */
    ~IServiceProvider() noexcept override = default;

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

class BLABBYUPNPAV_EXPORT IServiceProviderFactory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(IServiceProviderFactory)
public:
    ~IServiceProviderFactory() override = default;
    virtual std::unique_ptr<IServiceProvider> createServiceProvider(const QString &searchTarget) = 0;

protected:
    IServiceProviderFactory() = default;
};

} // namespace UPnPAV

#endif // ISERVICEPROVIDER_H
