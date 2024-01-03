// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONNECTIONMANAGERACTIONS_H
#define CONNECTIONMANAGERACTIONS_H

#include "SCPDAction.hpp"

// clang-format off
namespace UPnPAV
{

SCPDAction GetProtocolInfo() noexcept;

SCPDAction GetCurrentConnectionIDs() noexcept;

SCPDAction GetCurrentConnectionInfo() noexcept;

} //namespace UPnPAV

// clang-format on

#endif // CONNECTIONMANAGERACTIONS_H
