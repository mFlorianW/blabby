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

SCPDStateVariable SourceProtocolInfo() noexcept
{
    return {
        true,
        "SourceProtocolInfo",
        SCPDStateVariable::DataType::String
    };
}

SCPDStateVariable SinkProtocolInfo() noexcept
{
    return {
        true,
        "SinkProtocolInfo",
        SCPDStateVariable::DataType::String
    };
}

SCPDStateVariable CurrentConnectionIDs() noexcept
{
    return {
        true,
        "CurrentConnectionIDs",
        SCPDStateVariable::DataType::String
    };
}

SCPDStateVariable A_ARG_TYPE_ConnectionStatus() noexcept
{
    return {
        false,
        "A_ARG_TYPE_ConnectionStatus",
        SCPDStateVariable::DataType::String,
        "",
        {"OK", "ContentFormatMismatch", "InsufficientBandwidth", "UnreliableChannel", "Unknown"}
    };
}

SCPDStateVariable A_ARG_TYPE_ConnectionManager() noexcept
{
    return {
        false,
        "A_ARG_TYPE_ConnectionManager",
        SCPDStateVariable::DataType::String,
    };
}

SCPDStateVariable A_ARG_TYPE_Direction() noexcept
{
    return {
        false,
        "A_ARG_TYPE_Direction",
        SCPDStateVariable::DataType::String,
        {},
        {"Input","Output"}
    };
}

SCPDStateVariable A_ARG_TYPE_ProtocolInfo() noexcept
{
    return {
        false,
        "A_ARG_TYPE_ProtocolInfo",
        SCPDStateVariable::DataType::String,
    };
}

SCPDStateVariable A_ARG_TYPE_ConnectionID() noexcept
{
    return {
        false,
        "A_ARG_TYPE_ConnectionID",
        SCPDStateVariable::DataType::I4,
    };
}

SCPDStateVariable A_ARG_TYPE_AVTransportID() noexcept
{
    return {
        false,
        "A_ARG_TYPE_AVTransportID",
        SCPDStateVariable::DataType::I4,
    };
}

SCPDStateVariable A_ARG_TYPE_RcsID() noexcept
{
    return {
        false,
        "A_ARG_TYPE_RcsID",
        SCPDStateVariable::DataType::I4,
    };
}

} //namespace UPnPAV

// clang-format on

#endif // CONNECTIONMANAGERSTATEVARIABLES_H
