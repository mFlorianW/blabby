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
#ifndef SERVICECONTROLPOINTDEFINITION_H
#define SERVICECONTROLPOINTDEFINITION_H

#include "SCPDStateVariable.h"
#include "SCPDAction.h"

namespace UPnPAV
{

class ServiceControlPointDefinition
{
public:
    ServiceControlPointDefinition();
    ServiceControlPointDefinition(const QString &scpdUrl,
                                  const QVector<SCPDStateVariable> &serviceStateTable,
                                  const QVector<SCPDAction> &actionList = {});

    const QString &scpdUrl() const;

    const QVector<SCPDStateVariable> &serviceStateTable() const;

    const QVector<SCPDAction> &actionList() const;

    friend bool operator==(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition& rhs);

    friend bool operator!=(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition& rhs);


private:
    QString m_scpdUrl;
    QVector<SCPDStateVariable> m_stateVariables;
    QVector<SCPDAction> m_actions;
};

} //namespace UPnPAV

#endif // SERVICECONTROLPOINTDEFINITION_H
