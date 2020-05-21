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
#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include "UPnP_Export.h"
#include "IconDescription.h"
#include "ServiceDescription.h"
#include "ServiceControlPointDefinition.h"

#include <QString>
#include <QVector>
#include <optional>

namespace UPnPAV
{
class UPNP_EXPORT DeviceDescription final
{
public:
    /**
     * Construcsts an default DeviceDescription. All members
     * are empty.
     */
    DeviceDescription();

    DeviceDescription(const QString &deviceType,
                      const QString &friendlyName,
                      const QString &manufacturer,
                      const QString &modelName,
                      const QString &udn,
                      const QVector<IconDescription> &icons = {},
                      const QVector<ServiceDescription> &services = {},
                      const QVector<ServiceControlPointDefinition> &scpds = {});

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

    friend bool operator==(const DeviceDescription &lhs, const DeviceDescription & rhs);
    friend bool operator!=(const DeviceDescription &lhs, const DeviceDescription & rhs);

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

}

#endif // DEVICEDESCRIPTION_H
