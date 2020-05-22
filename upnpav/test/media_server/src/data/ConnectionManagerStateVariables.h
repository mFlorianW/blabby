#ifndef CONNECTIONMANAGERSTATEVARIABLES_H
#define CONNECTIONMANAGERSTATEVARIABLES_H

#include "SCPDStateVariable.h"

namespace UPnPAV
{

SCPDStateVariable SourceProtocolInfo
{
    true,
    "SourceProtocolInfo",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable SinkProtocolInfo
{
    true,
    "SinkProtocolInfo",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable CurrentConnectionIDs
{
    true,
    "CurrentConnectionConnectionIDs",
    SCPDStateVariable::DataType::String
};

SCPDStateVariable A_ARG_TYPE_ConnectionStatus
{
    false,
    "A_ARG_TYPE_ConnectionStatus",
    SCPDStateVariable::DataType::String,
    "",
    {"OK", "ContentFormatMismatch", "InsufficientBandwidth", "UnreliableChannel", "Unknown"}
};

SCPDStateVariable A_ARG_TYPE_ConnectionManager
{
    false,
    "A_ARG_TYPE_ConnectionManager",
    SCPDStateVariable::DataType::String,
};

SCPDStateVariable A_ARG_TYPE_Direction
{
    false,
    "A_ARG_TYPE_Direction",
    SCPDStateVariable::DataType::String,
    {},
    {"Input","Output"}
};

SCPDStateVariable A_ARG_TYPE_ProtocolInfo
{
    false,
    "A_ARG_TYPE_ProtocolInfo",
    SCPDStateVariable::DataType::String,
};

SCPDStateVariable A_ARG_TYPE_ConnectionID
{
    false,
    "A_ARG_TYPE_ConnectionID",
    SCPDStateVariable::DataType::I4,
};

SCPDStateVariable A_ARG_TYPE_AVTransportID
{
    false,
    "A_ARG_TYPE_AVTransportID",
    SCPDStateVariable::DataType::I4,
};

SCPDStateVariable A_ARG_TYPE_RcsID
{
    false,
    "A_ARG_TYPE_RcsID",
    SCPDStateVariable::DataType::I4,
};

} //namespace UPnPAV

#endif // CONNECTIONMANAGERSTATEVARIABLES_H
