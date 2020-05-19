#include "MediaServer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"

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
    validateContentDirectoryDescription();

    auto connectionManagerDescription = deviceDescription.service("urn:schemas-upnp-org:service:ConnectionManager");
    if(!connectionManagerDescription)
    {
        throw InvalidDeviceDescription{"ConnectionManager description not found."};
    }

    m_ConnectionServiceDescription = connectionManagerDescription.value();
    validateConnectionManagerDescription();
}

void MediaServer::validateConnectionManagerDescription()
{
    if(m_ConnectionServiceDescription.eventUrl().isEmpty())
    {
        throw InvalidDeviceDescription{"ConnectionManager event URL is not set."};
    }

    if(m_ConnectionServiceDescription.controlUrl().isEmpty())
    {
        throw InvalidDeviceDescription{"ConnectionManager control URL is not set."};
    }

    if(m_ConnectionServiceDescription.id().isEmpty())
    {
        throw InvalidDeviceDescription{"ConnectionManager service ID is not set."};
    }
}

void MediaServer::validateContentDirectoryDescription()
{
    if(m_contentDirectoryDescription.eventUrl().isEmpty())
    {
        throw InvalidDeviceDescription("ContentDirectory event URL is not set");
    }

    if(m_contentDirectoryDescription.controlUrl().isEmpty())
    {
        throw InvalidDeviceDescription("ContentDirectory control URL is not set.");
    }

    if(m_contentDirectoryDescription.id().isEmpty())
    {
        throw InvalidDeviceDescription("ContentDirectory service ID is not set.");
    }
}

} //namespace UPnPAV
