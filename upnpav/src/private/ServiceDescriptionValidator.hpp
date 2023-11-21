// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDESCRIPTIONVALIDATOR_H
#define SERVICEDESCRIPTIONVALIDATOR_H

#include "BaseValidator.hpp"
#include "ServiceDescription.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

/**
 * Validator for a ServiceDescription checks that every required
 * flag is set.
 */
class BLABBYUPNPAV_EXPORT ServiceDescriptionValidator : public BaseValidator
{
public:
    /**
     * Consturcts the ServiceDescriptionValidator.
     *
     * @param descriptionName The name of the description used for the error message.
     * @param serviceDescription The ServiceDescription that shall be validate.
     */
    ServiceDescriptionValidator(QString descriptionName, ServiceDescription serviceDescription);

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

} // namespace UPnPAV

#endif // SERVICEDESCRIPTIONVALIDATOR_H
