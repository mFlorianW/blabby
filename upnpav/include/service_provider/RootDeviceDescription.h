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
#ifndef PHYSICALDEVICEDESCRIPTION_H
#define PHYSICALDEVICEDESCRIPTION_H

#include "DeviceDescription.h"
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

class RootDeviceDescription
{
public:
    RootDeviceDescription();

    RootDeviceDescription(const QVector<DeviceDescription> &devices,
                          const QVector<ServiceControlPointDefinition> &scpds = {});

    const QVector<DeviceDescription> &devices() const noexcept;

    const QVector<ServiceControlPointDefinition> &scpds() const noexcept;

    friend bool operator==(const RootDeviceDescription &lhs, const RootDeviceDescription & rhs);
    friend bool operator!=(const RootDeviceDescription &lhs, const RootDeviceDescription & rhs);

private:
    QVector<DeviceDescription> m_embeddedDevices;
    QVector<ServiceControlPointDefinition> m_scpds;
};

} // namespace UPnPAV

#endif // PHYSICALDEVICEDESCRIPTION_H
