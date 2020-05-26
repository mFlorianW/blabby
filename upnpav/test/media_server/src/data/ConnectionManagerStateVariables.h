/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
    "CurrentConnectionIDs",
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
