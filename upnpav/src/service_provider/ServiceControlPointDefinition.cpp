// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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

bool ServiceControlPointDefinition::hasStateVariable(const QString &stateVariableName) const noexcept
{
    for (const auto &variable : m_stateVariables)
    {
        if (variable.name() == stateVariableName)
        {
            return true;
        }
    }

    return false;
}

const QVector<SCPDStateVariable> &ServiceControlPointDefinition::serviceStateTable() const
{
    return m_stateVariables;
}

const QVector<SCPDAction> &ServiceControlPointDefinition::actionList() const
{
    return m_actions;
}

SCPDAction ServiceControlPointDefinition::action(const QString &actionName) const noexcept
{
    for (const auto &action : m_actions)
    {
        if (action.name() == actionName)
        {
            return action;
        }
    }

    return {};
}

bool operator==(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition &rhs)
{
    if (&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_actions == rhs.m_actions) && (lhs.m_stateVariables == rhs.m_stateVariables));
}

bool operator!=(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
