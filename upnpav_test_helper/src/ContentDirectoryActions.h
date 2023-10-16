// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONTENTDIRECTORYACTIONS_H
#define CONTENTDIRECTORYACTIONS_H

#include "SCPDAction.h"

// clang-format off
namespace UPnPAV
{

static SCPDAction GetSearchCapabilities
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

static SCPDAction GetSortCapabilities
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

static SCPDAction GetSystemUpdateID
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

static SCPDAction Browse
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
        SCPDArgument
        {
            "UpdateID",
            SCPDArgument::Direction::Out,
            "A_ARG_TYPE_UpdateID"
        }
    }
};

} //namespace UPnPAV

// clang-format on

#endif // CONTENTDIRECTORYACTIONS_H
