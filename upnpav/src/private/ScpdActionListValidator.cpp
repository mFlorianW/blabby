// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ScpdActionListValidator.hpp"

#include <utility>

namespace UPnPAV
{

ScpdActionListValidator::ScpdActionListValidator(QString scpdName,
                                                 ServiceControlPointDefinition scpd,
                                                 QVector<QString> actionNames)
    : m_scpdName(std::move(scpdName))
    , m_scpd(std::move(scpd))
    , m_actionNames(std::move(actionNames))
{
}

bool ScpdActionListValidator::validate() noexcept
{
    for (auto &actionName : m_actionNames) {
        auto iter = std::find_if(m_scpd.actionList().begin(), m_scpd.actionList().end(), [=](SCPDAction const &action) {
            if (actionName == action.name()) {
                return true;
            }

            return false;
        });
        if (iter == m_scpd.actionList().end()) {
            m_errorMessage = QString{"%1 SCPD has no %2 action"}.arg(m_scpdName, actionName);
            return false;
        }
    }

    return true;
}

} // namespace UPnPAV
