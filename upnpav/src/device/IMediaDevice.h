// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef IMEDIADEVICE_H
#define IMEDIADEVICE_H

#include "blabbyupnpav_export.h"
#include <QObject>

namespace UPnPAV
{

/**
 * This interface defines functions that every upnp media device
 * should support.
 */
class BLABBYUPNPAV_EXPORT IMediaDevice
{
public:
    virtual ~IMediaDevice() = default;

    /**
     * Gives the human friendly name of the device.
     *
     * @return The human friendly name of the device.
     */
    virtual const QString &name() const noexcept = 0;

    /**
     * Gives the Url of an icon for the device. The string
     * can be empty when the device doesn't have an icon.
     *
     * @return The a Url to an icon of the device.
     */
    virtual const QUrl &iconUrl() const noexcept = 0;
};

} // namespace UPnPAV

Q_DECLARE_INTERFACE(UPnPAV::IMediaDevice, "IMediaDevice")

#endif // IMEDIADEVICE_H
