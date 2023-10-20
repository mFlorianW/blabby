// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "ContentDirectoryStateVariables.h"

namespace UPnPAV
{
SCPDStateVariable A_ARG_TYPE_ObjectID()
{
    static auto var =
        SCPDStateVariable{false, QStringLiteral("A_ARG_TYPE_ObjectID"), SCPDStateVariable::DataType::String};
    return var;
}
SCPDStateVariable A_ARG_TYPE_Result()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_Result", SCPDStateVariable::DataType::String};
    return var;
}

SCPDStateVariable A_ARG_TYPE_BrowseFlag()
{
    static auto var = SCPDStateVariable{false,
                                        "A_ARG_TYPE_BrowseFlag",
                                        SCPDStateVariable::DataType::String,
                                        "",
                                        {"BrowseMetaData"
                                         "BrowseDirectChildren"}};
    return var;
}

SCPDStateVariable A_ARG_TYPE_Filter()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_Filter", SCPDStateVariable::DataType::String};
    return var;
}

SCPDStateVariable A_ARG_TYPE_SortCriteria()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_SortCriteria", SCPDStateVariable::DataType::String};
    return var;
}
SCPDStateVariable A_ARG_TYPE_Index()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_Index", SCPDStateVariable::DataType::String};
    return var;
}

SCPDStateVariable A_ARG_TYPE_Count()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_Count", SCPDStateVariable::DataType::Ui4};
    return var;
}

SCPDStateVariable A_ARG_TYPE_UpdateID()
{
    static auto var = SCPDStateVariable{false, "A_ARG_TYPE_UpdateID", SCPDStateVariable::DataType::Ui4};
    return var;
}

SCPDStateVariable SearchCapabilities()
{
    static auto var = SCPDStateVariable{false, "SearchCapabilities", SCPDStateVariable::DataType::String};
    return var;
}

SCPDStateVariable SortCapabilities()
{
    static auto var = SCPDStateVariable{false, "SortCapabilities", SCPDStateVariable::DataType::String};
    return var;
}

SCPDStateVariable SystemUpdateID()
{
    static auto var = SCPDStateVariable{true, "SystemUpdateID", SCPDStateVariable::DataType::Ui4};
    return var;
}
} // namespace UPnPAV
