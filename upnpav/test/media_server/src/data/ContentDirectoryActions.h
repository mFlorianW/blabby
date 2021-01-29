// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
