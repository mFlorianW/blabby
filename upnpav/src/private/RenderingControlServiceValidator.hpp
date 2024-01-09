// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "private/ServiceValidatorBase.hpp"

namespace UPnPAV
{
class RenderingControlServiceValidator : public ServiceValidatorBase
{
public:
    RenderingControlServiceValidator(DeviceDescription const& desc);
};

} // namespace UPnPAV
