#ifndef ISERVICEPROVIDER_H
#define ISERVICEPROVIDER_H

#include "UPnP_Export.h"
#include <QObject>

namespace UPnPAV
{
class DeviceDescription;
class ServiceProviderError;

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