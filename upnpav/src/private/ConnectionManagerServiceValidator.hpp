// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONNECTIONMANAGERVALIDATOR_H
#define CONNECTIONMANAGERVALIDATOR_H

#include "BaseValidator.hpp"
#include "DeviceDescription.hpp"
#include "ServiceValidatorBase.hpp"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT ConnectionManagerServiceValidator : public ServiceValidatorBase
{
public:
    ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription);
};

} // namespace UPnPAV

#endif // CONNECTIONMANAGERVALIDATOR_H
