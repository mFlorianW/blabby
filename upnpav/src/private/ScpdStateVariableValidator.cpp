// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ScpdStateVariableValidator.hpp"

#include <utility>

namespace UPnPAV
{

ScpdStateVariableValidator::ScpdStateVariableValidator(QString scpdName,
                                                       ServiceControlPointDefinition scpd,
                                                       QVector<QString> stateVariableNames)
    : m_scpdName(std::move(scpdName))
    , m_scpd(std::move(scpd))
    , m_stateVariableNames(std::move(stateVariableNames))
{
}

bool ScpdStateVariableValidator::validate() noexcept
{
    for (auto &variableName : m_stateVariableNames) {
        auto iter = std::find_if(m_scpd.serviceStateTable().begin(),
                                 m_scpd.serviceStateTable().end(),
                                 [=](SCPDStateVariable const &variable) {
                                     if (variableName == variable.name()) {
                                         return true;
                                     }

                                     return false;
                                 });
        if (iter == m_scpd.serviceStateTable().end()) {
            m_errorMessage = QString{"%1 SCPD has no %2 variable"}.arg(m_scpdName, variableName);
            return false;
        }
    }

    return true;
}

} // namespace UPnPAV
