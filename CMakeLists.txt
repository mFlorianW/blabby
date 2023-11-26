# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.11)

project(Blabby
    VERSION 0.1.0
    DESCRIPTION "Blabby UPnP control point for Raspberry PI"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/configuration/cmake/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# Modules
################################################################################
include(GenerateExportHeader)
include(FeatureSummary)

################################################################################
# Options
################################################################################
enable_testing()
option(ENABLE_TEST "Enable/Disable tests build. Default: false" OFF)
option(BUILD_DOC "Enable/Disable documentation build. Default: false" OFF)

################################################################################
# Variables
################################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BASE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

################################################################################
# Subdirectories
################################################################################
add_subdirectory(upnpav)
add_subdirectory(shell)
add_subdirectory(providers)
add_subdirectory(multimedia)

if (ENABLE_TEST)
    add_subdirectory(upnpav_test_helper)
endif()

if(BUILD_DOC)
    add_subdirectory(docs)
endif()

list(REMOVE_DUPLICATES QML_IMPORT_PATH)
feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
