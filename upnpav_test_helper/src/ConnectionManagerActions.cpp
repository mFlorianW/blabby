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

SCPDAction GetProtocolInfo() noexcept
{
    return { 
        "GetProtocolInfo",
        {
            SCPDArgument
            {
                "Source",
                SCPDArgument::Direction::Out,
                "SourceProtocolInfo"
            },

            SCPDArgument
            {
                "Sink",
                SCPDArgument::Direction::Out,
                "SinkProtocolInfo"
            }
        }
    };
}

SCPDAction GetCurrentConnectionIDs() noexcept
{
    return {
        "GetCurrentConnectionIDs",
        {
            SCPDArgument
            {
                "ConnectionIDs",
                SCPDArgument::Direction::Out,
                "CurrentConnectionIDs"
            }
        }
    };
}

SCPDAction GetCurrentConnectionInfo() noexcept
{
    return {
        "GetCurrentConnectionInfo",
        {
            SCPDArgument
            {
                "ConnectionID",
                SCPDArgument::Direction::In,
                "A_ARG_TYPE_ConnectionID"
            },
            SCPDArgument
            {
                "RcsID",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_RcsID"
            },
            SCPDArgument
            {
                "AVTransportID",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_AVTransportID"
            },
            SCPDArgument
            {
                "ProtocolInfo",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_ProtocolInfo"
            },
            SCPDArgument
            {
                "PeerConnectionManager",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_ConnectionManager"
            },
            SCPDArgument
            {
                "PeerConnectionID",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_ConnectionID"
            },
            SCPDArgument
            {
                "Direction",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_Direction"
            },
            SCPDArgument
            {
                "Status",
                SCPDArgument::Direction::Out,
                "A_ARG_TYPE_ConnectionStatus"
            },
        }
    };
}

} //namespace UPnPAV

// clang-format on

#endif // CONNECTIONMANAGERACTIONS_H
