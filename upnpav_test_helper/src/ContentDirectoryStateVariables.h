// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONTENTDIRECTORYSTATEVARIABLES_H
#define CONTENTDIRECTORYSTATEVARIABLES_H

#include "SCPDStateVariable.h"

// clang-format off
namespace UPnPAV
{

static SCPDStateVariable A_ARG_TYPE_ObjectID
{
    false,
    "A_ARG_TYPE_ObjectID",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_Result
{
    false,
    "A_ARG_TYPE_Result",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_BrowseFlag
{
    false,
    "A_ARG_TYPE_BrowseFlag",
    SCPDStateVariable::DataType::String,
    "",
    {
        "BrowseMetaData"
        "BrowseDirectChildren"
    }
};

static SCPDStateVariable A_ARG_TYPE_Filter
{
    false,
    "A_ARG_TYPE_Filter",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_SortCriteria
{
    false,
    "A_ARG_TYPE_SortCriteria",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_Index
{
    false,
    "A_ARG_TYPE_Index",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable A_ARG_TYPE_Count
{
    false,
    "A_ARG_TYPE_Count",
    SCPDStateVariable::DataType::Ui4
};

static SCPDStateVariable A_ARG_TYPE_UpdateID
{
    false,
    "A_ARG_TYPE_UpdateID",
    SCPDStateVariable::DataType::Ui4
};

static SCPDStateVariable SearchCapabilities
{
    false,
    "SearchCapabilities",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable SortCapabilities
{
    false,
    "SortCapabilities",
    SCPDStateVariable::DataType::String
};

static SCPDStateVariable SystemUpdateID
{
    true,
    "SystemUpdateID",
    SCPDStateVariable::DataType::Ui4
};

} //namespace UPnPAV

// clang-format on


#endif // CONTENTDIRECTORYSTATEVARIABLES_H
