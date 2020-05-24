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
#ifndef SCPDSTATEVARIAABLEVALIDATOR_H
#define SCPDSTATEVARIAABLEVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

class ScpdStateVariableValidator final : public BaseValidator
{
public:
    /**
     * Construst the SCPDSateVariableValidator. After construction call
     * validate to check for all variables.
     *
     * @param scpdName The name of the SCPD.
     * @param scpd The SCPD itself
     * @param stateVariableNames A list of variable name that shall be checked.
     */
    ScpdStateVariableValidator(const QString &scpdName,
                               const ServiceControlPointDefinition &scpd,
                               const QVector<QString> &stateVariableNames);
    ~ScpdStateVariableValidator() override;

    /**
     * Checks if the SCPD contains all the variables given in the variable name vector.
     *
     * @return True all variable names are found.
     */
    bool validate() noexcept override;

private:
    QString m_scpdName;
    ServiceControlPointDefinition m_scpd;
    QVector<QString> m_stateVariableNames;
};

} //namespace UPnPAV

#endif // SCPDSTATEVARIAABLEVALIDATOR_H
