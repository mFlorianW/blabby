// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include "ServiceValidatorBase.h"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT AvTransportServiceValidator : public ServiceValidatorBase
{
public:
    AvTransportServiceValidator(DeviceDescription &deviceDescription);
};

} // namespace UPnPAV
