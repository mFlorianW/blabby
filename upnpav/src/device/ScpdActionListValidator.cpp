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
    for(auto &actionName : m_actionNames)
    {
        auto iter = std::find_if(m_scpd.actionList().begin(),
                                 m_scpd.actionList().end(),
                                 [=](const SCPDAction &action)
                                 {
                                    if(actionName == action.name())
                                    {
                                        return true;
                                    }

                                    return false;
                                 });
        if(iter == m_scpd.actionList().end())
        {
            m_errorMessage = QString{"%1 SCPD has no %2 action"}.arg(m_scpdName).arg(actionName);
            return false;
        }
    }

    return true;
}

} //namespace UPnPAV
