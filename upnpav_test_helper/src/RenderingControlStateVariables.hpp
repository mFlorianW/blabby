// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "SCPDStateVariable.hpp"

namespace UPnPAV
{
SCPDStateVariable lastChangeVariable() noexcept;

SCPDStateVariable presetNameListVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_ChannelVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_InstanceIDVariable() noexcept;

SCPDStateVariable a_ARG_TYPE_PresetNameVariable() noexcept;

SCPDStateVariable volume() noexcept;

} // namespace UPnPAV
