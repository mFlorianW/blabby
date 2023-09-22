// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDSTATEVARIAABLEVALIDATOR_H
#define SCPDSTATEVARIAABLEVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceControlPointDefinition.h"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT ScpdStateVariableValidator final : public BaseValidator
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

} // namespace UPnPAV

#endif // SCPDSTATEVARIAABLEVALIDATOR_H
