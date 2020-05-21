#ifndef CONNECTIONMANAGERACTIONS_H
#define CONNECTIONMANAGERACTIONS_H

#include "SCPDAction.h"

namespace UPnPAV
{

SCPDAction GetProtocolInfo
{
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

SCPDAction GetCurrentConnectionIDs
{
    "GetCurrentConnectionIDs",
    {
        SCPDArgument
        {
            "ConnectionID",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_ConnectionID"
        }
    }
};

SCPDAction GetCurrentConnectionInfo
{
    "GetCurrentConnectionInfo",
    {
        SCPDArgument
        {
            "ConnectionID",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_ ConnectionID"
        },

        SCPDArgument
        {
            "ConnectionID",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_ ConnectionID"
        },
    }
};

} //namespace UPnPAV

#endif // CONNECTIONMANAGERACTIONS_H
