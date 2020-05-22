#ifndef SERVICEDESCRIPTIONVALIDATOR_H
#define SERVICEDESCRIPTIONVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceDescription.h"

namespace UPnPAV
{

/**
 * Validator for a ServiceDescription checks that every required
 * flag is set.
 */
class ServiceDescriptionValidator : public BaseValidator
{
public:
    /**
     * Consturcts the ServiceDescriptionValidator.
     *
     * @param descriptionName The name of the description used for the error message.
     * @param serviceDescription The ServiceDescription that shall be validate.
     */
    ServiceDescriptionValidator(const QString descriptionName,
                                const ServiceDescription &serviceDescription);

    /**
     * Validates all components of the ServiceDescription so that they are not
     * empty.
     *
     * @return True all okay, otherwise false.
     */
    bool validate() noexcept override;

private:
    QString m_descriptionName;
    ServiceDescription m_serviceDescription;
};

} //namespace UPnPAV

#endif // SERVICEDESCRIPTIONVALIDATOR_H
