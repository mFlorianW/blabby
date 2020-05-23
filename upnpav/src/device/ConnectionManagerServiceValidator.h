#ifndef CONNECTIONMANAGERVALIDATOR_H
#define CONNECTIONMANAGERVALIDATOR_H

#include "BaseValidator.h"
#include "DeviceDescription.h"
#include "ServiceValidatorBase.h"

namespace UPnPAV
{

class ConnectionManagerServiceValidator : public ServiceValidatorBase
{
public:
    ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription);
};

} //namespace UPnPAV

#endif // CONNECTIONMANAGERVALIDATOR_H
