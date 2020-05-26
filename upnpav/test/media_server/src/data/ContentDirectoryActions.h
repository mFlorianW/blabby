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
#ifndef CONTENTDIRECTORYACTIONS_H
#define CONTENTDIRECTORYACTIONS_H

#include "SCPDAction.h"

namespace UPnPAV
{
SCPDAction GetSearchCapabilities
{
    "GetSearchCapabilities",
    {
        SCPDArgument
        {
            "SearchCaps",
            SCPDArgument::Direction::Out,
            "SearchCapabilities"
        }
    }
};

SCPDAction GetSortCapabilities
{
    "GetSortCapabilities",
    {
        SCPDArgument
        {
            "SortCaps",
            SCPDArgument::Direction::Out,
            "SortCapabilities"
        }
    }
};

SCPDAction GetSystemUpdateID
{
    "GetSystemUpdateID",
    {
        SCPDArgument
        {
            "Id",
            SCPDArgument::Direction::Out,
            "SystemUpdateID"
        }
    }
};

SCPDAction Browse
{
    "Browse",
    {
        SCPDArgument
        {
            "ObjectID",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_ObjectID"
        },
        SCPDArgument
        {
            "BrowseFlag",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_BrowseFlag"
        },
        SCPDArgument
        {
            "Filter",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_Filter"
        },
        SCPDArgument
        {
            "StartingIndex",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_Index"
        },
        SCPDArgument
        {
            "RequestedCount",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_Count"
        },
        SCPDArgument
        {
            "SortCriteria",
            SCPDArgument::Direction::In,
            "A_ARG_TYPE_SortCriteria"
        },
        SCPDArgument
        {
            "Result",
            SCPDArgument::Direction::Out,
            "A_ARG_TYPE_Result"
        },
        SCPDArgument
        {
            "NumberReturned",
            SCPDArgument::Direction::Out,
            "A_ARG_TYPE_Count"
        },
        SCPDArgument
        {
            "TotalMatches",
            SCPDArgument::Direction::Out,
            "A_ARG_TYPE_Count"
        },
    }
};

} //namespace UPnPAV

#endif // CONTENTDIRECTORYACTIONS_H
