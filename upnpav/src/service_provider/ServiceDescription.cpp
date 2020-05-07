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
#include "ServiceDescription.h"

namespace UPnPAV {

ServiceDescription::ServiceDescription()
{
}

ServiceDescription::ServiceDescription(const QString &serviceType,
                                       const QString &id,
                                       const QString &scpdUrl,
                                       const QString &controlUrl,
                                       const QString &eventUrl)
    : m_serviceType(serviceType)
    , m_id(id)
    , m_scpdUrl(scpdUrl)
    , m_controlUrl(controlUrl)
    , m_eventUrl(eventUrl)
{
}

QString ServiceDescription::serviceType() const
{
    return m_serviceType;
}

QString ServiceDescription::id() const
{
    return m_id;
}

QString ServiceDescription::scpdUrl() const
{
    return m_scpdUrl;
}

QString ServiceDescription::controlUrl() const
{
    return m_controlUrl;
}

QString ServiceDescription::eventUrl() const
{
    return m_eventUrl;
}

bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_serviceType == rhs.m_serviceType) &&
            (lhs.m_id == rhs.m_id) &&
            (lhs.m_scpdUrl == rhs.m_scpdUrl) &&
            (lhs.m_controlUrl == rhs.m_controlUrl) &&
            (lhs.m_eventUrl == rhs.m_eventUrl));
}

bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    return !(lhs == rhs);
}

} //namespace UPnPAV
