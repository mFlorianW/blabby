// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
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
                      QVector<ServiceControlPointDefinition> scpds = {});

    const QString &deviceType() const noexcept;

    const QString &friendlyName() const noexcept;

    const QString &manufacturer() const noexcept;

    const QVector<IconDescription> &icons() const noexcept;

    const QString &modelName() const noexcept;

    const QString &udn() const noexcept;

    std::optional<ServiceDescription> service(const QString &serviceName) const noexcept;
    const QVector<ServiceDescription> &services() const noexcept;

    const QVector<ServiceControlPointDefinition> &scpds() const noexcept;

    std::optional<ServiceControlPointDefinition> scpd(const QString &scpdUrl) const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(const DeviceDescription &lhs, const DeviceDescription &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(const DeviceDescription &lhs, const DeviceDescription &rhs);

private:
    QString m_deviceType{""};
    QString m_friendlyName{""};
    QString m_manufacturer{""};
    QString m_modelName{""};
    QString m_udn{""};

    QVector<IconDescription> m_icons;
    QVector<ServiceDescription> m_services;
    QVector<ServiceControlPointDefinition> m_scpds;
};

} // namespace UPnPAV

Q_DECLARE_METATYPE(UPnPAV::DeviceDescription)

#endif // DEVICEDESCRIPTION_H
