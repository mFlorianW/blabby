// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONNECTIONMANAGERSTATEVARIABLES_H
#define CONNECTIONMANAGERSTATEVARIABLES_H

#include "SCPDStateVariable.hpp"

// clang-format off
namespace UPnPAV
{

SCPDStateVariable SourceProtocolInfo() noexcept;

SCPDStateVariable SinkProtocolInfo() noexcept;

SCPDStateVariable CurrentConnectionIDs() noexcept;

SCPDStateVariable A_ARG_TYPE_ConnectionStatus() noexcept;

SCPDStateVariable A_ARG_TYPE_ConnectionManager() noexcept;

SCPDStateVariable A_ARG_TYPE_Direction() noexcept;

SCPDStateVariable A_ARG_TYPE_ProtocolInfo() noexcept;

SCPDStateVariable A_ARG_TYPE_ConnectionID() noexcept;

SCPDStateVariable A_ARG_TYPE_AVTransportID() noexcept;

SCPDStateVariable A_ARG_TYPE_RcsID() noexcept;

} //namespace UPnPAV

// clang-format on

#endif // CONNECTIONMANAGERSTATEVARIABLES_H
