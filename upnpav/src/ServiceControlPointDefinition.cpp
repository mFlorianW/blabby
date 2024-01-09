// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceControlPointDefinition.hpp"

namespace UPnPAV
{

ServiceControlPointDefinition::ServiceControlPointDefinition() = default;

ServiceControlPointDefinition::ServiceControlPointDefinition(QString scpdUrl,
                                                             QVector<SCPDStateVariable> stateVariables,
                                                             QVector<SCPDAction> actions)
    : d{new ServiceControlPointDefinitionData{std::move(scpdUrl), std::move(stateVariables), std::move(actions)}}
{
}

QString const& ServiceControlPointDefinition::scpdUrl() const
{
    return d->mScpdUrl;
}

bool ServiceControlPointDefinition::hasStateVariable(QString const& stateVariableName) const noexcept
{
    for (auto const& variable : std::as_const(d->mStateVariables)) {
        if (variable.name() == stateVariableName) {
            return true;
        }
    }

    return false;
}

QVector<SCPDStateVariable> const& ServiceControlPointDefinition::serviceStateTable() const
{
    return d->mStateVariables;
}

QVector<SCPDAction> const& ServiceControlPointDefinition::actionList() const
{
    return d->mActions;
}

SCPDAction ServiceControlPointDefinition::action(QString const& actionName) const noexcept
{
    for (auto const& action : std::as_const(d->mActions)) {
        if (action.name() == actionName) {
            return action;
        }
    }

    return {};
}

bool operator==(ServiceControlPointDefinition const& lhs, ServiceControlPointDefinition const& rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.d->mActions == rhs.d->mActions) && (lhs.d->mStateVariables == rhs.d->mStateVariables));
}

bool operator!=(ServiceControlPointDefinition const& lhs, ServiceControlPointDefinition const& rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
