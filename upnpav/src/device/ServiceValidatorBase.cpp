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
