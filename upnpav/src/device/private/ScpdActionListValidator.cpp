// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ScpdActionListValidator.h"

namespace UPnPAV
{

ScpdActionListValidator::ScpdActionListValidator(const QString &scpdName,
                                                 const ServiceControlPointDefinition &scpd,
                                                 const QVector<QString> actionNames)
    : m_scpdName(scpdName)
    , m_scpd(scpd)
    , m_actionNames(actionNames)
{
}

ScpdActionListValidator::~ScpdActionListValidator()
{
}

bool ScpdActionListValidator::validate() noexcept
{
    for (auto &actionName : m_actionNames)
    {
        auto iter = std::find_if(m_scpd.actionList().begin(), m_scpd.actionList().end(), [=](const SCPDAction &action) {
            if (actionName == action.name())
            {
                return true;
            }

            return false;
        });
        if (iter == m_scpd.actionList().end())
        {
            m_errorMessage = QString{"%1 SCPD has no %2 action"}.arg(m_scpdName).arg(actionName);
            return false;
        }
    }

    return true;
}

} // namespace UPnPAV
