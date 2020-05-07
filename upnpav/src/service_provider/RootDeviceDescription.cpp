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
#include "RootDeviceDescription.h"

namespace UPnPAV
{

RootDeviceDescription::RootDeviceDescription()
{
}

RootDeviceDescription::RootDeviceDescription(const QVector<DeviceDescription> &embeddedDevices,
                                             const QVector<ServiceControlPointDefinition> &scpds)
    : m_embeddedDevices(embeddedDevices)
    , m_scpds(scpds)
{
}

const QVector<DeviceDescription> &RootDeviceDescription::devices() const noexcept
{
    return m_embeddedDevices;
}

const QVector<ServiceControlPointDefinition> &RootDeviceDescription::scpds() const noexcept
{
    return m_scpds;
}

bool operator==(const RootDeviceDescription &lhs,
                const RootDeviceDescription &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_embeddedDevices == rhs.m_embeddedDevices) &&
            (lhs.m_scpds == rhs.m_scpds));
}

bool operator!=(const RootDeviceDescription &lhs, const RootDeviceDescription &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
