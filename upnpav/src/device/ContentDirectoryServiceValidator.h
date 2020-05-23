#ifndef CONTENTDIRECTORYSERVICEVALIDATOR_H
#define CONTENTDIRECTORYSERVICEVALIDATOR_H

#include "ServiceValidatorBase.h"
#include "DeviceDescription.h"

namespace UPnPAV
{

class ContentDirectoryServiceValidator final : public ServiceValidatorBase
{
public:
    ContentDirectoryServiceValidator(const DeviceDescription &deviceDescription);

};

} //namespace UPnPAV

#endif // CONTENTDIRECTORYVALIDATOR_H
