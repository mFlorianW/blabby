#include "ConnectionManagerServiceValidator.h"
#include "ScpdStateVariableValidator.h"
#include "ServiceDescriptionValidator.h"
#include "ScpdActionListValidator.h"

namespace UPnPAV
{
static constexpr char CONNECTION_MANAGER[] = "ConnectionManager";

ConnectionManagerServiceValidator::ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription)
    : m_deviceDescription(deviceDescription)
{
}

bool ConnectionManagerServiceValidator::validate() noexcept
{
    return hasConnectionManagerService() &&
           validateServiceDescription() &&
           hasSCPD() &&
           validateStateVariables() &&
           validateActions();
}

bool ConnectionManagerServiceValidator::hasConnectionManagerService()
{
    auto serviceDescription = m_deviceDescription.service("urn:schemas-upnp-org:service:ConnectionManager");
    if(!serviceDescription)
    {
        m_errorMessage = QString{"ConnectionManager description not found."};
        return false;
    }

    m_serviceDescription = serviceDescription.value();
    return true;
}

bool ConnectionManagerServiceValidator::validateServiceDescription()
{
    ServiceDescriptionValidator serviceDescriptionValidator
    {
        CONNECTION_MANAGER,
        m_serviceDescription
    };

    if(!serviceDescriptionValidator.validate())
    {
        m_errorMessage = serviceDescriptionValidator.errorMessage();
        return false;
    }

    return true;
}

bool ConnectionManagerServiceValidator::hasSCPD()
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

bool ConnectionManagerServiceValidator::validateStateVariables()
{
    ScpdStateVariableValidator stateVariableValidator
    {
        CONNECTION_MANAGER,
        m_serviceControlPointDefinition,
        {
            "SourceProtocolInfo",
            "SinkProtocolInfo",
            "CurrentConnectionConnectionIDs",
            "A_ARG_TYPE_ConnectionStatus",
            "A_ARG_TYPE_ConnectionManager",
            "A_ARG_TYPE_Direction",
            "A_ARG_TYPE_ProtocolInfo",
            "A_ARG_TYPE_ConnectionID",
            "A_ARG_TYPE_RcsID"
        }
    };

    if(!stateVariableValidator.validate())
    {
        m_errorMessage = stateVariableValidator.errorMessage();
        return false;
    }

    return true;
}

bool ConnectionManagerServiceValidator::validateActions()
{
    ScpdActionListValidator actionVariableValidator
    {
        CONNECTION_MANAGER,
        m_serviceControlPointDefinition,
        {
            "GetProtocolInfo",
            "GetCurrentConnectionIDs",
            "GetCurrentConnectionInfo"
        }
    };

    if(!actionVariableValidator.validate())
    {
        m_errorMessage = actionVariableValidator.errorMessage();
        return false;
    }

    return true;
}

} //namespace
