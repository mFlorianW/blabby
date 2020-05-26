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
#include "ServiceValidatorBase.h"
#include "ScpdStateVariableValidator.h"
#include "ScpdActionListValidator.h"
#include "ServiceDescriptionValidator.h"

namespace UPnPAV
{

ServiceValidatorBase::~ServiceValidatorBase()
{
}

bool ServiceValidatorBase::validate() noexcept
{
    return hasService() &&
           validateServiceDescription() &&
           hasSCPD() &&
           validateStateVariables() &&
            validateActions();
}

ServiceDescription ServiceValidatorBase::serviceDescription() const noexcept
{
    return m_serviceDescription;
}

bool ServiceValidatorBase::hasService()
{
    auto serviceDescription = m_deviceDescription.service(m_serviceType);
    if(!serviceDescription)
    {
        m_errorMessage = QString{m_serviceName + " description not found."};
        return false;
    }

    m_serviceDescription = serviceDescription.value();
    return true;
}

bool ServiceValidatorBase::validateServiceDescription()
{
    ServiceDescriptionValidator serviceDescriptionValidator
    {
        m_serviceName,
        m_serviceDescription
    };

    if(!serviceDescriptionValidator.validate())
    {
        m_errorMessage = serviceDescriptionValidator.errorMessage();
        return false;
    }

    return true;
}

bool ServiceValidatorBase::hasSCPD()
{
    auto scpdConnectionManager = m_deviceDescription.scpd(m_serviceDescription.scpdUrl());
    if(!scpdConnectionManager)
    {
        m_errorMessage = QString{"ConnectionManager SCPD not found."};
        return false;
    }

    m_serviceControlPointDefinition = scpdConnectionManager.value();
    return true;
}

bool ServiceValidatorBase::validateStateVariables()
{
    ScpdStateVariableValidator stateVariableValidator
    {
        m_serviceName,
        m_serviceControlPointDefinition,
        m_stateVariables
    };

    if(!stateVariableValidator.validate())
    {
        m_errorMessage = stateVariableValidator.errorMessage();
        return false;
    }

    return true;
}

bool ServiceValidatorBase::validateActions()
{
    ScpdActionListValidator actionVariableValidator
    {
        m_serviceName,
        m_serviceControlPointDefinition,
        m_actions
    };

    if(!actionVariableValidator.validate())
    {
        m_errorMessage = actionVariableValidator.errorMessage();
        return false;
    }

    return true;
}

} //namespace UPnPAV
