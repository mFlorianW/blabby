// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "SCPDStateVariable.h"

namespace UPnPAV
{
SCPDStateVariable lastChangeVariable() noexcept;

SCPDStateVariable presetNameListVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_ChannelVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_InstanceIDVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_PresetNameVariable() noexcept;

} // namespace UPnPAV
