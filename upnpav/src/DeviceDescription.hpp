// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include "IconDescription.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"
#include "blabbyupnpav_export.h"

#include <QMetaType>
#include <QString>
#include <QVector>
#include <optional>

namespace UPnPAV
{
class BLABBYUPNPAV_EXPORT DeviceDescription final
{
public:
    /**
     * Construcsts an default DeviceDescription. All members
     * are empty.
     */
    DeviceDescription();

    DeviceDescription(QString deviceType,
                      QString friendlyName,
                      QString manufacturer,
                      QString modelName,
                      QString udn,
                      QVector<IconDescription> icons = {},
                      QVector<ServiceDescription> services = {},
                      QVector<ServiceControlPointDefinition> scpds = {},
                      QString address = {});

    QString const& deviceType() const noexcept;

    QString const& friendlyName() const noexcept;

    QString const& manufacturer() const noexcept;

    QVector<IconDescription> const& icons() const noexcept;

    QString const& modelName() const noexcept;

    QString const& udn() const noexcept;

    /**
     * Gives the network address of the device, the host of the location the description was fetched from.
     * The address is not part of the description document and therefore not considered by the comparison.
     * @return The address of the device or an empty string when it is unknown.
     */
    QString const& address() const noexcept;

    std::optional<ServiceDescription> service(QString const& serviceName) const noexcept;
    QVector<ServiceDescription> const& services() const noexcept;

    QVector<ServiceControlPointDefinition> const& scpds() const noexcept;

    std::optional<ServiceControlPointDefinition> scpd(QString const& scpdUrl) const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(DeviceDescription const& lhs, DeviceDescription const& rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(DeviceDescription const& lhs, DeviceDescription const& rhs);

private:
    QString m_deviceType{""};
    QString m_friendlyName{""};
    QString m_manufacturer{""};
    QString m_modelName{""};
    QString m_udn{""};
    QString m_address{""};

    QVector<IconDescription> m_icons;
    QVector<ServiceDescription> m_services;
    QVector<ServiceControlPointDefinition> m_scpds;
};

} // namespace UPnPAV

Q_DECLARE_METATYPE(UPnPAV::DeviceDescription)

#endif // DEVICEDESCRIPTION_H
