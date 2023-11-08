# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later
################################################################################
# Project
################################################################################
cmake_minimum_required(VERSION 3.11)

project(BlabbyMediaServerProvider
    VERSION 0.1.0
    DESCRIPTION "Blabby Mutlitmedia component"
    LANGUAGES CXX)

###############################################################################
# Options
###############################################################################
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN yes)

###############################################################################
# Modules 
###############################################################################
include(GNUInstallDirs)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick)

################################################################################
# MediaServer
################################################################################
add_subdirectory(src)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()

