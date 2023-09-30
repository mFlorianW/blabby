// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "PendingSoapCall.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QUrl>

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

protected:
    MediaDevice(DeviceDescription deviceDescription, QSharedPointer<SoapMessageTransmitter> msgTransmitter);

private:
    QScopedPointer<MediaDevicePrivate> d;
};

} // namespace UPnPAV
