// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONNECTIONMANAGERSTATEVARIABLES_H
#define CONNECTIONMANAGERSTATEVARIABLES_H

#include "SCPDStateVariable.h"

// clang-format off
namespace UPnPAV
{

static SCPDStateVariable SourceProtocolInfo
{
    true,
    "SourceProtocolInfo",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable SinkProtocolInfo
{
    true,
    "SinkProtocolInfo",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable CurrentConnectionIDs
{
    true,
    "CurrentConnectionIDs",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_ConnectionStatus
{
    false,
    "A_ARG_TYPE_ConnectionStatus",
    SCPDStateVariable::DataType::String,
    "",
    {"OK", "ContentFormatMismatch", "InsufficientBandwidth", "UnreliableChannel", "Unknown"}
};

static SCPDStateVariable A_ARG_TYPE_ConnectionManager
{
    false,
    "A_ARG_TYPE_ConnectionManager",
    SCPDStateVariable::DataType::String,
};

static SCPDStateVariable A_ARG_TYPE_Direction
{
    false,
    "A_ARG_TYPE_Direction",
    SCPDStateVariable::DataType::String,
    {},
    {"Input","Output"}
};

static SCPDStateVariable A_ARG_TYPE_ProtocolInfo
{
    false,
    "A_ARG_TYPE_ProtocolInfo",
    SCPDStateVariable::DataType::String,
};

static SCPDStateVariable A_ARG_TYPE_ConnectionID
{
    false,
    "A_ARG_TYPE_ConnectionID",
    SCPDStateVariable::DataType::I4,
};

static SCPDStateVariable A_ARG_TYPE_AVTransportID
{
    false,
    "A_ARG_TYPE_AVTransportID",
    SCPDStateVariable::DataType::I4,
};

static SCPDStateVariable A_ARG_TYPE_RcsID
{
    false,
    "A_ARG_TYPE_RcsID",
    SCPDStateVariable::DataType::I4,
};

} //namespace UPnPAV

// clang-format on

#endif // CONNECTIONMANAGERSTATEVARIABLES_H
