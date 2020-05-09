#include "MediaServer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"

namespace UPnPAV
{

MediaServer::MediaServer(const DeviceDescription &deviceDescription)
{
    if(!hasConnectionServiceDescription(deviceDescription))
    {
        throw InvalidDeviceDescription{};
    }

    if(!hasContentDirectoryServiceDescription(deviceDescription))
    {
        throw InvalidDeviceDescription{};
    }
}

bool MediaServer::hasContentDirectoryServiceDescription(const DeviceDescription &deviceDescription) const noexcept
{
    if(deviceDescription.hasServiceType(QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory")))
    {
        return true;
    }

    return false;
}

bool MediaServer::hasConnectionServiceDescription(const DeviceDescription &deviceDescription) const noexcept
{
    if(deviceDescription.hasServiceType(QStringLiteral("urn:schemas-upnp-org:service:ConnectionManager")))
    {
        return true;
    }

    return false;
}

} //namespace UPnPAV
