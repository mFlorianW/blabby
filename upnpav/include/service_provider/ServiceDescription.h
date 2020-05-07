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
#ifndef SERVICEDESCRIPTION_H
#define SERVICEDESCRIPTION_H

#include <QString>

namespace UPnPAV
{

class ServiceDescription
{
public:
    ServiceDescription();

    ServiceDescription(const QString& serviceType,
                       const QString& id,
                       const QString& scpdUrl,
                       const QString& controlUrl,
                       const QString& eventUrl);

    QString serviceType() const;

    QString id() const;

    QString scpdUrl() const;

    QString controlUrl() const;

    QString eventUrl() const;

    friend bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs);
    friend bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs);

private:
    QString m_serviceType;
    QString m_id;
    QString m_scpdUrl;
    QString m_controlUrl;
    QString m_eventUrl;
};

} //namespace UPnPAV

#endif // SERVICEDESCRIPTION_H
