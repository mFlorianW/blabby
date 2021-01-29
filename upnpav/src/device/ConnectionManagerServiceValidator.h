// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONNECTIONMANAGERVALIDATOR_H
#define CONNECTIONMANAGERVALIDATOR_H

#include "BaseValidator.h"
#include "DeviceDescription.h"
#include "ServiceValidatorBase.h"

namespace UPnPAV
{

class ConnectionManagerServiceValidator : public ServiceValidatorBase
{
public:
    ConnectionManagerServiceValidator(const DeviceDescription &deviceDescription);
};

} //namespace UPnPAV

#endif // CONNECTIONMANAGERVALIDATOR_H
