// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "SCPDAction.h"

namespace UPnPAV
{
SCPDAction listPresetsAction() noexcept;

SCPDAction selectPresetAction() noexcept;
}
