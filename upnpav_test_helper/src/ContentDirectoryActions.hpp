// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#ifndef CONTENTDIRECTORYACTIONS_H
#define CONTENTDIRECTORYACTIONS_H

#include "SCPDAction.hpp"

// clang-format off
namespace UPnPAV
{

SCPDAction GetSearchCapabilities();

SCPDAction GetSortCapabilities();

SCPDAction GetSystemUpdateID();

SCPDAction Browse();

} //namespace UPnPAV

// clang-format on

#endif // CONTENTDIRECTORYACTIONS_H
