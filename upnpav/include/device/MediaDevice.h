// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIADEVICE_H
#define MEDIADEVICE_H

#include "IMediaDevice.h"
#include "UPnP_Export.h"
#include <QObject>
#include <memory>

namespace UPnPAV
{
class MediaDevicePrivate;
class DeviceDescription;

class UPNP_EXPORT MediaDevice : public IMediaDevice
{
    Q_INTERFACES(UPnPAV::IMediaDevice)
public:
    MediaDevice(const DeviceDescription &deviceDescription);
    virtual ~MediaDevice();

private:
    std::unique_ptr<MediaDevicePrivate> d;
};

} // namespace UPnPAV

#endif // MEDIADEVICE_H
