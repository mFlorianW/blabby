#include "MediaServer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "ConnectionManagerServiceValidator.h"

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

    ConnectionManagerServiceValidator conManagerServiceValidator{deviceDescription};
    if(!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
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
