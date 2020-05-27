/**
 ** This file is part of the Blabby project.
 ** Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
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
#ifndef SERVICEVALIDATORBASE_H
#define SERVICEVALIDATORBASE_H

#include "BaseValidator.h"
#include "DeviceDescription.h"

namespace UPnPAV
{
/**
 * The base class for every service validator. Subclasses only need to
 * fill the setup the m_serviceName, m_serviceType, m_stateVariables
 * and m_action variables.
 */
class ServiceValidatorBase : public BaseValidator
{
public:
    ~ServiceValidatorBase() override;
    /**
     * Validates the service and checks that the device description
     * contains the service, state variables and actions.
     *
     * @return True device description contains the service,
     *         otherwise false.
     */
    bool validate() noexcept override;

    ServiceDescription serviceDescription() const noexcept;

    ServiceControlPointDefinition scpd() const noexcept;

protected:
    DeviceDescription m_deviceDescription;

    QString m_serviceName;
    QString m_serviceType;
    QVector<QString> m_stateVariables;
    QVector<QString> m_actions;

private:
    /**
     * Checks the device description for the servier.
     *
     * @return True device description contains the service, otherwise false.
     */
    bool hasService();

    /**
     * Validates the service description for it's correctness.
     *
     * @return True ServiceDescription is valid, otherwise false.
     */
    bool validateServiceDescription();

    /**
     * Checks the device description if it has a SCPD or not.
     *
     * @return True SCPD found, otherwise false.
     */
    bool hasSCPD();

    /**
     * Checks the SCPD for the required state variables.
     *
     * @return True all variables found, otherwise false.
     */
    bool validateStateVariables();

    /**
     * Checks the SCPD for the required actions.
     *
     * @return True all actions found otherwise false.
     */
    bool validateActions();
private:
    ServiceDescription m_serviceDescription;
    ServiceControlPointDefinition m_serviceControlPointDefinition;
};

} //namespace UPnPAV

#endif // SERVICEVALIDATORBASE_H
