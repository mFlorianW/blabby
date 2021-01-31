#include "MediaDevice.h"
#include "ConnectionManagerServiceValidator.h"
#include "InvalidDeviceDescription.h"
#include "MediaDevice_p.h"

namespace UPnPAV
{

MediaDevice::MediaDevice(const DeviceDescription &deviceDescription)
    : d(std::make_unique<MediaDevicePrivate>())
{
    ConnectionManagerServiceValidator conManagerServiceValidator{ deviceDescription };
    if(!conManagerServiceValidator.validate())
    {
        throw InvalidDeviceDescription{ conManagerServiceValidator.errorMessage() };
    }
}

MediaDevice::~MediaDevice() = default;

} // namespace UPnPAV
