# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.11)

project(Blabby
    VERSION 0.1.0
    DESCRIPTION "Blabby UPnP control point for Raspberry PI"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)

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

################################################################################
# Variables
################################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BASE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Install folder
set(DEFAULT_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(DEFAULT_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(DEFAULT_MULTIMEDIA_PLUGIN_INSTALL_DIR ${DEFAULT_LIBRARY_INSTALL_DIR}/plugins/multimedia)
set(DEFAULT_QML_PLUGIN_INSTALL_DIR ${DEFAULT_LIBRARY_INSTALL_DIR}/plugins/qml)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "QtCreator QML import paths" FORCE)

################################################################################
# Compiler-Settings
################################################################################
if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g3 -fno-inline)

        if(ENABLE_TEST)
            #add_compile_options(-fprofile-arcs -ftest-coverage)
        endif()
    else()
        add_compile_options(-O2)
    endif()

    add_compile_options(-Wall -fPIC -Wextra -Wunused-function -fvisibility=hidden -fmessage-length=0)
endif()

add_definitions("-DQT_NO_KEYWORDS")
add_definitions("-DINSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

################################################################################
# Subdirectories
################################################################################
add_subdirectory(upnpav)
add_subdirectory(plugincore)
add_subdirectory(shell)
add_subdirectory(plugins)

if (ENABLE_TEST)
    add_subdirectory(upnpav_test_helper)
endif()

list(REMOVE_DUPLICATES QML_IMPORT_PATH)
feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
