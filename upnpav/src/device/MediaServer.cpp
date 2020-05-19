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

    auto connectionManagerDescription = deviceDescription.service("urn:schemas-upnp-org:service:ConnectionManager");
    if(!connectionManagerDescription)
    {
        throw InvalidDeviceDescription{"ConnectionManager description not found."};
    }
}

} //namespace UPnPAV
