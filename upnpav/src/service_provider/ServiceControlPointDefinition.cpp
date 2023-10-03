// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

ServiceControlPointDefinition::ServiceControlPointDefinition() = default;

ServiceControlPointDefinition::ServiceControlPointDefinition(QString scpdUrl,
                                                             QVector<SCPDStateVariable> stateVariables,
                                                             QVector<SCPDAction> actions)
    : d{new ServiceControlPointDefinitionData{std::move(scpdUrl), std::move(stateVariables), std::move(actions)}}
{
}

const QString &ServiceControlPointDefinition::scpdUrl() const
{
    return d->mScpdUrl;
}

bool ServiceControlPointDefinition::hasStateVariable(const QString &stateVariableName) const noexcept
{
    for (const auto &variable : std::as_const(d->mStateVariables))
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
    return d->mStateVariables;
}

const QVector<SCPDAction> &ServiceControlPointDefinition::actionList() const
{
    return d->mActions;
}

SCPDAction ServiceControlPointDefinition::action(const QString &actionName) const noexcept
{
    for (const auto &action : std::as_const(d->mActions))
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

    return ((lhs.d->mActions == rhs.d->mActions) && (lhs.d->mStateVariables == rhs.d->mStateVariables));
}

bool operator!=(const ServiceControlPointDefinition &lhs, const ServiceControlPointDefinition &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
