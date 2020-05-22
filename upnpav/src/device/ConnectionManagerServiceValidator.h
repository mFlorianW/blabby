#ifndef CONNECTIONMANAGERVALIDATOR_H
#define CONNECTIONMANAGERVALIDATOR_H

#include "BaseValidator.h"
#include "DeviceDescription.h"

namespace UPnPAV
{

class ConnectionManagerServiceValidator : public BaseValidator
{
public:
    ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription);

    bool validate() noexcept override;

private:
    bool hasConnectionManagerService();

    bool validateServiceDescription();

    bool hasSCPD();

    bool validateStateVariables();

    bool validateActions();

private:
    DeviceDescription m_deviceDescription;
    ServiceDescription m_serviceDescription;
    ServiceControlPointDefinition m_serviceControlPointDefinition;
};

} //namespace UPnPAV

#endif // CONNECTIONMANAGERVALIDATOR_H
