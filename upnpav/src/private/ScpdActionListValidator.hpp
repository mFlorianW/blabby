// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDACTIONLISTVALIDATOR_H
#define SCPDACTIONLISTVALIDATOR_H

#include "BaseValidator.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{
/**
 * Validator for action list in the SCPD description.
 */
class BLABBYUPNPAV_EXPORT ScpdActionListValidator final : public BaseValidator
{
public:
    /**
     * Constructs a SCPDActionListValidator.
     *
     * @param scpdName The name of SCPD. Used for error message.
     * @param scpd The SCPD that shall be checked.
     * @param actionNames The name of the action for which shall be checked.
     */
    ScpdActionListValidator(QString scpdName, ServiceControlPointDefinition scpd, QVector<QString> actionNames);

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

} // namespace UPnPAV

#endif // SCPDACTIONLISTVALIDATOR_H
