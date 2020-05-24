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
#include "ScpdStateVariableValidator.h"

namespace UPnPAV
{

ScpdStateVariableValidator::ScpdStateVariableValidator(const QString &scpdName,
                                                       const ServiceControlPointDefinition &scpd,
                                                       const QVector<QString> &stateVariableNames)
    : m_scpdName(scpdName)
    , m_scpd(scpd)
    , m_stateVariableNames(stateVariableNames)
{
}

ScpdStateVariableValidator::~ScpdStateVariableValidator()
{
}

bool ScpdStateVariableValidator::validate() noexcept
{
    for(auto &variableName : m_stateVariableNames)
    {
        auto iter = std::find_if(m_scpd.serviceStateTable().begin(),
                                 m_scpd.serviceStateTable().end(),
                                 [=](const SCPDStateVariable &variable)
                                 {
                                    if(variableName == variable.name())
                                    {
                                        return true;
                                    }

                                    return false;
                                 });
        if(iter == m_scpd.serviceStateTable().end())
        {
            m_errorMessage = QString{"%1 SCPD has no %2 variable"}.arg(m_scpdName).arg(variableName);
            return false;
        }
    }

    return true;
}

} //namespace UPnPAV

