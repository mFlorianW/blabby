# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later
################################################################################
# Project
################################################################################
cmake_minimum_required(VERSION 3.11)

project(BlabbyShell
    VERSION 0.1.0
    DESCRIPTION "Blabby Mutlitmedia component"
    LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Modules 
###############################################################################
include(GNUInstallDirs)
include(CompilerSettings)

###############################################################################
# Variables
###############################################################################
set(DEFAULT_PROVIDER_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/blabby/plugins)

###############################################################################
# Runtime path
###############################################################################
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
endif("${isSystemDir}" STREQUAL "-1")

################################################################################
# Dependencies
################################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

###############################################################################
# Polices
###############################################################################
qt_policy(SET QTP0001 NEW)

################################################################################
# Blabby
################################################################################
add_subdirectory(src)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()
