// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "SCPDAction.hpp"

namespace UPnPAV
{
SCPDAction listPresetsAction() noexcept;

SCPDAction selectPresetAction() noexcept;
} // namespace UPnPAV
