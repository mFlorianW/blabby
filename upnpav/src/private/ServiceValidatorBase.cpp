// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceValidatorBase.h"
#include "ScpdActionListValidator.h"
#include "ScpdStateVariableValidator.h"
#include "ServiceDescriptionValidator.h"

namespace UPnPAV
{

ServiceValidatorBase::~ServiceValidatorBase()
{
}

bool ServiceValidatorBase::validate() noexcept
{
    return hasService() && validateServiceDescription() && hasSCPD() && validateStateVariables() && validateActions();
}

ServiceDescription ServiceValidatorBase::serviceDescription() const noexcept
{
    return m_serviceDescription;
}

ServiceControlPointDefinition ServiceValidatorBase::scpd() const noexcept
{
    return m_serviceControlPointDefinition;
}

bool ServiceValidatorBase::hasService()
{
    auto serviceDescription = m_deviceDescription.service(m_serviceType);
    if (!serviceDescription)
    {
        m_errorMessage = QString{m_serviceName + " description not found."};
        return false;
    }

    m_serviceDescription = serviceDescription.value();
    return true;
}

bool ServiceValidatorBase::validateServiceDescription()
{
    ServiceDescriptionValidator serviceDescriptionValidator{m_serviceName, m_serviceDescription};

    if (!serviceDescriptionValidator.validate())
    {
        m_errorMessage = serviceDescriptionValidator.errorMessage();
        return false;
    }

    return true;
}

bool ServiceValidatorBase::hasSCPD()
{
    auto scpdConnectionManager = m_deviceDescription.scpd(m_serviceDescription.scpdUrl());
    if (!scpdConnectionManager)
    {
        m_errorMessage = QString{"ConnectionManager SCPD not found."};
        return false;
    }

    m_serviceControlPointDefinition = scpdConnectionManager.value();
    return true;
}

bool ServiceValidatorBase::validateStateVariables()
{
    ScpdStateVariableValidator stateVariableValidator{m_serviceName, m_serviceControlPointDefinition, m_stateVariables};

    if (!stateVariableValidator.validate())
    {
        m_errorMessage = stateVariableValidator.errorMessage();
        return false;
    }

    return true;
}

bool ServiceValidatorBase::validateActions()
{
    ScpdActionListValidator actionVariableValidator{m_serviceName, m_serviceControlPointDefinition, m_actions};

    if (!actionVariableValidator.validate())
    {
        m_errorMessage = actionVariableValidator.errorMessage();
        return false;
    }

    return true;
}

} // namespace UPnPAV
