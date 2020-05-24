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
#ifndef SCPDACTIONLISTVALIDATOR_H
#define SCPDACTIONLISTVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{
/**
 * Validator for action list in the SCPD description.
 */
class ScpdActionListValidator final : public BaseValidator
{
public:
    /**
     * Constructs a SCPDActionListValidator.
     *
     * @param scpdName The name of SCPD. Used for error message.
     * @param scpd The SCPD that shall be checked.
     * @param actionNames The name of the action for which shall be checked.
     */
    ScpdActionListValidator(const QString &scpdName,
                            const ServiceControlPointDefinition &scpd,
                            const QVector<QString> actionNames);
    ~ScpdActionListValidator() override;

    /**
     * Checks if the SCPD contains all the action names.
     *
     * @return True all action names are present, otherwise false.
     */
    bool validate() noexcept override;

private:
    QString m_scpdName;
    ServiceControlPointDefinition m_scpd;
    QVector<QString> m_actionNames;
};

} //namespace UPnPAV

#endif // SCPDACTIONLISTVALIDATOR_H
