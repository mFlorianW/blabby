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
