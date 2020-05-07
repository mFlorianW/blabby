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
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

ServiceControlPointDefinition::ServiceControlPointDefinition()
{

}

ServiceControlPointDefinition::ServiceControlPointDefinition(const QString &scpdUrl,
                                                             const QVector<SCPDStateVariable> &stateVariables,
                                                             const QVector<SCPDAction> &actions)
    : m_scpdUrl(scpdUrl)
    , m_stateVariables(stateVariables)
    , m_actions(actions)
{
}

const QString &ServiceControlPointDefinition::scpdUrl() const
{
    return m_scpdUrl;
}

const QVector<SCPDStateVariable> &ServiceControlPointDefinition::serviceStateTable() const
{
    return m_stateVariables;
}

const QVector<SCPDAction> &ServiceControlPointDefinition::actionList() const
{
    return m_actions;
}

bool operator==(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_actions == rhs.m_actions) &&
            (lhs.m_stateVariables == rhs.m_stateVariables));
}

bool operator!=(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition &rhs)
{
    return !(lhs == rhs);
}

} //namespace UPnPAV
