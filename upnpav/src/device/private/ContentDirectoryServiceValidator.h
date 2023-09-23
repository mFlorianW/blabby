// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONTENTDIRECTORYSERVICEVALIDATOR_H
#define CONTENTDIRECTORYSERVICEVALIDATOR_H

#include "DeviceDescription.h"
#include "ServiceValidatorBase.h"
#include "blabbyupnpav_export.h"

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT ContentDirectoryServiceValidator final : public ServiceValidatorBase
{
public:
    ContentDirectoryServiceValidator(const DeviceDescription &deviceDescription);
};

} // namespace UPnPAV

#endif // CONTENTDIRECTORYVALIDATOR_H
