#include "MediaServer.h"
#include "DeviceDescription.h"
#include "InvalidDeviceDescription.h"
#include "ConnectionManagerServiceValidator.h"
#include "ContentDirectoryServiceValidator.h"

namespace UPnPAV
{

MediaServer::MediaServer(const DeviceDescription &deviceDescription)
{
    ConnectionManagerServiceValidator conManagerServiceValidator{deviceDescription};
    if(!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conManagerServiceValidator.errorMessage()};
    }

    ContentDirectoryServiceValidator conDirectoryServiceValidator{deviceDescription};
    if(!conDirectoryServiceValidator.validate())
    {
        throw InvalidDeviceDescription{conDirectoryServiceValidator.errorMessage()};
    }
}

} //namespace UPnPAV
