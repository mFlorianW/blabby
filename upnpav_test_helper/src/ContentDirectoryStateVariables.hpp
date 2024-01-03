// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CONTENTDIRECTORYSTATEVARIABLES_H
#define CONTENTDIRECTORYSTATEVARIABLES_H

#include "SCPDStateVariable.hpp"

// clang-format off
namespace UPnPAV
{

SCPDStateVariable A_ARG_TYPE_ObjectID();

SCPDStateVariable A_ARG_TYPE_Result();

SCPDStateVariable A_ARG_TYPE_BrowseFlag();

SCPDStateVariable A_ARG_TYPE_Filter();

SCPDStateVariable A_ARG_TYPE_SortCriteria();

SCPDStateVariable A_ARG_TYPE_Index();

SCPDStateVariable A_ARG_TYPE_Count();

SCPDStateVariable A_ARG_TYPE_UpdateID();

SCPDStateVariable SearchCapabilities();

SCPDStateVariable SortCapabilities();

SCPDStateVariable SystemUpdateID();

} //namespace UPnPAV

// clang-format on

#endif // CONTENTDIRECTORYSTATEVARIABLES_H
