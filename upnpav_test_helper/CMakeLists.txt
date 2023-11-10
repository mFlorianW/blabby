# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.15)

project(BlabbyUPnPAV
    DESCRIPTION "UPnP/AV TestHelerp for the implementation in Blabby"
    VERSION 0.2.0
)

###############################################################################
# Modules
###############################################################################
include(CompilerSettings)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Source
###############################################################################
add_subdirectory(src)
