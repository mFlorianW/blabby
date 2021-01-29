// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONTENTDIRECTORYSERVICEVALIDATOR_H
#define CONTENTDIRECTORYSERVICEVALIDATOR_H

#include "ServiceValidatorBase.h"
#include "DeviceDescription.h"

namespace UPnPAV
{

class ContentDirectoryServiceValidator final : public ServiceValidatorBase
{
public:
    ContentDirectoryServiceValidator(const DeviceDescription &deviceDescription);
};

} //namespace UPnPAV

#endif // CONTENTDIRECTORYVALIDATOR_H
