#include "MediaServer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "ScpdStateVariableValidator.h"

namespace UPnPAV
{

MediaServer::MediaServer(const DeviceDescription &deviceDescription)
{
    auto contentDirectoryDescription = deviceDescription.service("urn:schemas-upnp-org:service:ContentDirectory");
    if(!contentDirectoryDescription)
    {
        throw InvalidDeviceDescription{"ContentDirectory description not found."};
    }

    m_contentDirectoryDescription = contentDirectoryDescription.value();
    validateServiceDescription(m_contentDirectoryDescription, "ContentDirectory");

    auto connectionManagerDescription = deviceDescription.service("urn:schemas-upnp-org:service:ConnectionManager");
    if(!connectionManagerDescription)
    {
        throw InvalidDeviceDescription{"ConnectionManager description not found."};
    }

    m_ConnectionServiceDescription = connectionManagerDescription.value();
    validateServiceDescription(m_ConnectionServiceDescription, "ConnectionManager");

    auto scpdConnectionManager = deviceDescription.scpd(m_ConnectionServiceDescription.scpdUrl());
    if(!scpdConnectionManager)
    {
        throw InvalidDeviceDescription{"ConnectionManager SCPD not found."};
    }

    ScpdStateVariableValidator connectionStateVariableValidator
    {
        "ConnectionManager",
        scpdConnectionManager.value(),
        {
            "SourceProtocolInfo",
            "SinkProtocolInfo",
            "CurrentConnectionConnectionIDs",
            "A_ARG_TYPE_ConnectionStatus",
            "A_ARG_TYPE_ConnectionManager"
        }
    };

    if(!connectionStateVariableValidator.validate())
    {
        throw InvalidDeviceDescription(connectionStateVariableValidator.errorMessage());
    }
}

void MediaServer::validateServiceDescription(const ServiceDescription &serviceDesc, const QString &serviceName)
{
    if(serviceDesc.eventUrl().isEmpty())
    {
        throw InvalidDeviceDescription{serviceName + " event URL is not set."};
    }

    if(serviceDesc.controlUrl().isEmpty())
    {
        throw InvalidDeviceDescription{serviceName + " control URL is not set."};
    }

    if(serviceDesc.id().isEmpty())
    {
        throw InvalidDeviceDescription{serviceName + " service ID is not set."};
    }

    if(serviceDesc.scpdUrl().isEmpty())
    {
        throw InvalidDeviceDescription{serviceName + " SCPD URL is not set."};
    }
}

} //namespace UPnPAV
