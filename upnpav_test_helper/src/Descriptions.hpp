// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef DESCRIPTIONS_H
#define DESCRIPTIONS_H

#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"

namespace UPnPAV
{

ServiceDescription validContentDirectoryDescription();

ServiceDescription eventUrlMissingInContentDirectoryDescription();

ServiceDescription controlUrlMissingInContentDirectoryDescription();

ServiceDescription serviceIdMissingInContentDirectoryDescription();

ServiceDescription scpdUrlMissingInContentDirectoryDescription();

ServiceDescription validConnectionManagerDescription();

ServiceDescription eventUrlMissingInConnectionManagerDescription();

ServiceDescription controlUrlMissingInConnectionManagerDescription();

ServiceDescription serviceIdMissingInConnectionManagerDescription();

ServiceDescription scpdUrlMissingInConnectionManagerDescription();

QVector<SCPDStateVariable> validConnectionManagerStateVariables();

QVector<SCPDAction> validConnectionManagerActions();

ServiceControlPointDefinition validConnectionManagerSCPD();

QVector<SCPDStateVariable> validContentDirectoryStateVariables();

QVector<SCPDAction> validContentDirectoryActions();

ServiceControlPointDefinition validContentDirectorySCPD();

QVector<SCPDStateVariable> validAvTransportStateVariables() noexcept;

QVector<SCPDAction> validAvTranportActions() noexcept;

ServiceControlPointDefinition validAvTranportServiceSCPD() noexcept;

ServiceDescription validAvTransportServiceDescription() noexcept;

ServiceDescription eventUrlMissingInAvTransportDescription() noexcept;

ServiceDescription serviceUrlMissingInAvTransportDescription() noexcept;

ServiceDescription scpdUrlMissingInAvTransportDescription() noexcept;

ServiceDescription controlUrlMissingInAvTransportDescription() noexcept;

QVector<SCPDStateVariable> validRenderingControlStateVariables() noexcept;

QVector<SCPDAction> validRenderingControlActions() noexcept;

ServiceControlPointDefinition validRenderingControlSCPD() noexcept;

ServiceDescription validRenderingControlServiceDescription() noexcept;

ServiceDescription scpdUrlMissingInRenderingControlDescription() noexcept;

ServiceDescription eventUrlMissingInRenderingControlDescription() noexcept;

ServiceDescription controlUrlMissingInRenderingControlDescription() noexcept;

ServiceDescription serviceUrlMissingInRenderingControlDescription() noexcept;

} // namespace UPnPAV

#endif // DESCRIPTIONS_H
