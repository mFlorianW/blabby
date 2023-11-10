# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later
################################################################################
# Project
################################################################################
cmake_minimum_required(VERSION 3.11)

project(BlabbyMultimedia
    VERSION 0.1.0
    DESCRIPTION "Blabby Mutlitmedia component"
    LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Options
###############################################################################
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN yes)

###############################################################################
# Modules 
###############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(CompilerSettings)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

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
# Subdirectories
################################################################################
add_subdirectory(src)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()
