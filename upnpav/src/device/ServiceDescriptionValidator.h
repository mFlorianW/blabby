/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
