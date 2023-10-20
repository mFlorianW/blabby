// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "ContentDirectoryActions.h"

namespace UPnPAV
{

SCPDAction GetSearchCapabilities()
{
    static auto action = SCPDAction{"GetSearchCapabilities",
                                    {SCPDArgument{"SearchCaps", SCPDArgument::Direction::Out, "SearchCapabilities"}}};
    return action;
}

SCPDAction GetSortCapabilities()
{
    static auto action =
        SCPDAction{"GetSortCapabilities", {SCPDArgument{"SortCaps", SCPDArgument::Direction::Out, "SortCapabilities"}}};
    return action;
}

SCPDAction GetSystemUpdateID()
{
    static auto action =
        SCPDAction{"GetSystemUpdateID", {SCPDArgument{"Id", SCPDArgument::Direction::Out, "SystemUpdateID"}}};
    return action;
}

SCPDAction Browse()
{
    static auto action =
        SCPDAction{"Browse",
                   {SCPDArgument{"ObjectID", SCPDArgument::Direction::In, "A_ARG_TYPE_ObjectID"},
                    SCPDArgument{"BrowseFlag", SCPDArgument::Direction::In, "A_ARG_TYPE_BrowseFlag"},
                    SCPDArgument{"Filter", SCPDArgument::Direction::In, "A_ARG_TYPE_Filter"},
                    SCPDArgument{"StartingIndex", SCPDArgument::Direction::In, "A_ARG_TYPE_Index"},
                    SCPDArgument{"RequestedCount", SCPDArgument::Direction::In, "A_ARG_TYPE_Count"},
                    SCPDArgument{"SortCriteria", SCPDArgument::Direction::In, "A_ARG_TYPE_SortCriteria"},
                    SCPDArgument{"Result", SCPDArgument::Direction::Out, "A_ARG_TYPE_Result"},
                    SCPDArgument{"NumberReturned", SCPDArgument::Direction::Out, "A_ARG_TYPE_Count"},
                    SCPDArgument{"TotalMatches", SCPDArgument::Direction::Out, "A_ARG_TYPE_Count"},
                    SCPDArgument{"UpdateID", SCPDArgument::Direction::Out, "A_ARG_TYPE_UpdateID"}}};
    return action;
}

} // namespace UPnPAV
