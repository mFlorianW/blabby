// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DeviceDescription.hpp"

#include <QDebug>
#include <utility>

namespace UPnPAV
{

DeviceDescription::DeviceDescription() = default;

DeviceDescription::DeviceDescription(QString deviceType,
                                     QString friendlyName,
                                     QString manufacturer,
                                     QString modelName,
                                     QString udn,
                                     QVector<IconDescription> icons,
                                     QVector<ServiceDescription> services,
                                     QVector<ServiceControlPointDefinition> scpds)
    : m_deviceType(std::move(deviceType))
    , m_friendlyName(std::move(friendlyName))
    , m_manufacturer(std::move(manufacturer))
    , m_modelName(std::move(modelName))
    , m_udn(std::move(udn))
    , m_icons(std::move(icons))
    , m_services(std::move(services))
    , m_scpds(std::move(scpds))
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

std::optional<ServiceDescription> DeviceDescription::service(const QString &serviceName) const noexcept
{
    for (const auto &serviceDesc : m_services) {
        if (serviceDesc.serviceType().contains(serviceName)) {
            return std::optional<ServiceDescription>{serviceDesc};
        }
    }

    return std::nullopt;
}

const QVector<ServiceDescription> &DeviceDescription::services() const noexcept
{
    return m_services;
}

const QVector<ServiceControlPointDefinition> &DeviceDescription::scpds() const noexcept
{
    return m_scpds;
}

std::optional<ServiceControlPointDefinition> DeviceDescription::scpd(const QString &scpdUrl) const noexcept
{
    for (const auto &scpd : m_scpds) {
        if (scpd.scpdUrl() == scpdUrl) {
            return scpd;
        }
    }

    return std::nullopt;
}

bool operator==(const DeviceDescription &lhs, const DeviceDescription &rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.m_deviceType == rhs.m_deviceType) && (lhs.m_friendlyName == rhs.m_friendlyName) &&
            (lhs.m_icons == rhs.m_icons) && (lhs.m_modelName == rhs.m_modelName) && (lhs.m_udn == rhs.m_udn) &&
            (lhs.m_manufacturer == rhs.m_manufacturer) && (lhs.m_services == rhs.m_services));
}

bool operator!=(const DeviceDescription &lhs, const DeviceDescription &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
