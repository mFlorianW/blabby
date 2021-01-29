// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICECONTROLPOINTDEFINITION_H
#define SERVICECONTROLPOINTDEFINITION_H

#include "UPnP_Export.h"
#include "SCPDStateVariable.h"
#include "SCPDAction.h"

namespace UPnPAV
{

class UPNP_EXPORT ServiceControlPointDefinition
{
public:
    ServiceControlPointDefinition();
    ServiceControlPointDefinition(const QString &scpdUrl,
                                  const QVector<SCPDStateVariable> &serviceStateTable,
                                  const QVector<SCPDAction> &actionList = {});

    const QString &scpdUrl() const;

    bool hasStateVariable(const QString &stateVariableName) const noexcept;
    const QVector<SCPDStateVariable> &serviceStateTable() const;

    const QVector<SCPDAction> &actionList() const;
    SCPDAction action(const QString &actionName) const noexcept;

    friend bool operator==(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition& rhs);

    friend bool operator!=(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition& rhs);


private:
    QString m_scpdUrl;
    QVector<SCPDStateVariable> m_stateVariables;
    QVector<SCPDAction> m_actions;
};

} //namespace UPnPAV

#endif // SERVICECONTROLPOINTDEFINITION_H
