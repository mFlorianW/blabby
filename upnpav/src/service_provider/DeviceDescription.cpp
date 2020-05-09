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
#include "DeviceDescription.h"

#include <QDebug>

namespace UPnPAV
{

DeviceDescription::DeviceDescription()
{
}

DeviceDescription::DeviceDescription(const QString &deviceType,
                                     const QString &friendlyName,
                                     const QString &manufacturer,
                                     const QString &modelName,
                                     const QString &udn,
                                     const QVector<IconDescription> &icons,
                                     const QVector<ServiceDescription> &services,
                                     const QVector<ServiceControlPointDefinition> &scpds)
    : m_deviceType(deviceType)
    , m_friendlyName(friendlyName)
    , m_manufacturer(manufacturer)
    , m_modelName(modelName)
    , m_udn(udn)
    , m_icons(icons)
    , m_services(services)
    , m_scpds(scpds)
{
}

const QString &DeviceDescription::deviceType() const noexcept
{
    return m_deviceType;
}

const QString &DeviceDescription::friendlyName() const noexcept
{
    return m_friendlyName;
}

const QString &DeviceDescription::manufacturer() const noexcept
{
    return m_manufacturer;
}

const QVector<IconDescription> &DeviceDescription::icons() const noexcept
{
    return m_icons;
}

const QString &DeviceDescription::modelName() const noexcept
{
    return m_modelName;
}

const QString &DeviceDescription::udn() const noexcept
{
    return m_udn;
}

bool DeviceDescription::hasServiceType(const QString &serviceType) const noexcept
{
    for(const auto &serviceDesc : m_services)
    {
        if(serviceDesc.serviceType().contains(serviceType))
        {
            return true;
        }
    }

    return false;
}

const QVector<ServiceDescription> &DeviceDescription::services() const noexcept
{
    return m_services;
}

const QVector<ServiceControlPointDefinition> &DeviceDescription::scpds() const noexcept
{
    return m_scpds;
}

bool operator==(const DeviceDescription &lhs, const DeviceDescription &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_deviceType == rhs.m_deviceType) &&
            (lhs.m_friendlyName == rhs.m_friendlyName) &&
            (lhs.m_icons == rhs.m_icons) &&
            (lhs.m_modelName == rhs.m_modelName) &&
            (lhs.m_udn == rhs.m_udn) &&
            (lhs.m_manufacturer == rhs.m_manufacturer) &&
            (lhs.m_services == rhs.m_services));
}

bool operator!=(const DeviceDescription &lhs, const DeviceDescription &rhs)
{
    return !(lhs == rhs);
}

} //namespace UPnPAV
