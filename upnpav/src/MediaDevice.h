// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "PendingSoapCall.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QUrl>
#include <optional>

namespace UPnPAV
{

class MediaDevicePrivate;
class DeviceDescription;
class SoapMessageTransmitter;

/**
 * This interface defines functions that every upnp media device
 * should support.
 */
class MediaDevice
{
public:
    Q_DISABLE_COPY_MOVE(MediaDevice)

    /**
     * Default destructor
     */
    virtual ~MediaDevice();

    /**
     * Gives the human friendly name of the device.
     *
     * @return The human friendly name of the device.
     */
    virtual const QString &name() const noexcept;

    /**
     * Gives the Url of an icon for the device. The string
     * can be empty when the device doesn't have an icon.
     *
     * @return The a Url to an icon of the device.
     */
    virtual const QUrl &iconUrl() const noexcept;

    /**
     * Calls the GetProtocolInfo on the ConnectionManager Interface of the UPnPAV device.
     *
     * @return PendingSoapCall with the result or an error.
     */
    virtual QScopedPointer<PendingSoapCall> protocolInfo() noexcept;

    /**
     * Calls the GetCurrentConnectionIds on the ConnectionManager of the UPnPAV device.
     * @return PendingSoapCall with the result or an error.
     */
    virtual QScopedPointer<PendingSoapCall> currentConnectionIds() noexcept;

    /**
     * Calls the GetCurrentConnectionInfo on the ConnectionManager of the UPnPAV device.
     * @return PendingSoapCall with the result or an error.
     */
    virtual QScopedPointer<PendingSoapCall> currentConnectionInfo(quint32 connectionId) noexcept;

    /**
     * Checks if the media device implements the AVTransport service.
     * @return true The device supports AVTransport service otherwise false.
     */
    bool hasAvTransportService() const noexcept;

    /**
     * Calls the AVTransportURI on the AVTransport service of the device.
     *
     * @return PendingSoapCall with the result or an error.
     * The optional will be null if the device doesn't have support for the AVTransport service.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     *
     */
    virtual std::optional<QScopedPointer<PendingSoapCall>> setAvTransportUri(quint32 instanceId,
                                                                             QString const &uri,
                                                                             QString const &uriMetaData = QString{
                                                                                 ""}) noexcept;

    /**
     * Call the GetMediaInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> mediaInfo(quint32 instanceId);

    /**
     * Calls the GetTransportInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> transportInfo(quint32 instanceId);

    /**
     * Calls the GetPositionInfo on the AVTransport service of the device.
     * @param instanceId Identifies the virtual instance of the AVTransport service to which the action applies
     * @return PendingSoapCall with the result or an error.
     *
     * @note
     * The function can only be called if the device has an AVTransport service.
     * Before calling the function check if the service exists with @ref<MediaDevice::hasAvTransportService>
     */
    virtual std::optional<std::unique_ptr<PendingSoapCall>> positionInfo(quint32 instanceId);

protected:
    MediaDevice(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitter> msgTransmitter);

private:
    QScopedPointer<MediaDevicePrivate> d;
};

} // namespace UPnPAV
