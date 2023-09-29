// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "IMediaDevice.h"

namespace UPnPAV
{
class MediaDevicePrivate;
class DeviceDescription;
class SoapMessageTransmitter;
class MediaDevice : public IMediaDevice
{
public:
    Q_DISABLE_COPY_MOVE(MediaDevice)

    /**
     * Default destructor
     */
    ~MediaDevice() override;

    /**
     * @copydoc IMediaDevice::name()
     */
    const QString &name() const noexcept override;

    /**
     * @copydoc IMediaDevice::iconUrl()
     */
    const QUrl &iconUrl() const noexcept override;

    /**
     * @copydoc IMediaDevice::protocolInfo()
     */
    QScopedPointer<PendingSoapCall> protocolInfo() noexcept override;

    /**
     * @copydoc IMediaDevice::currentConnectionIds()
     */
    QScopedPointer<PendingSoapCall> currentConnectionIds() noexcept override;

    /**
     * @copydoc IMediaDevice::currentConnectionIds()
     */
    QScopedPointer<PendingSoapCall> currentConnectionInfo(quint32 connectionId) noexcept override;

protected:
    MediaDevice(DeviceDescription const &deviceDescription, QSharedPointer<SoapMessageTransmitter> msgTransmitter);

private:
    QScopedPointer<MediaDevicePrivate> d;
};

} // namespace UPnPAV
