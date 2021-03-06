// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef IMEDIADEVICE_H
#define IMEDIADEVICE_H

#include <QObject>

namespace UPnPAV
{

class IMediaDevice
{
public:
    virtual ~IMediaDevice() = default;
    virtual const QString &name() const noexcept = 0;
};

} // namespace UPnPAV

Q_DECLARE_INTERFACE(UPnPAV::IMediaDevice, "IMediaDevice")

#endif // IMEDIADEVICE_H
