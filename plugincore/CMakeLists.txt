# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.12)

project(LibBlabbyPluginCore
    DESCRIPTION "Plugin Core of the Blabby project"
    VERSION 0.1.0
)

################################################################################
# Build PluginCore
################################################################################
add_library(BlabbyPluginCore SHARED)
add_library(Blabby::PluginCore ALIAS BlabbyPluginCore)
generate_export_header(BlabbyPluginCore)

target_sources(BlabbyPluginCore
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MultimediaPlugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/MultimediaPlugin_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MultimediaPlugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PluginVersion.h
)

target_link_libraries(BlabbyPluginCore
    Qt5::Core
)

target_include_directories(BlabbyPluginCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

set_target_properties(BlabbyPluginCore PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()

################################################################################
# Install plugin
################################################################################
install(TARGETS BlabbyPluginCore DESTINATION ${DEFAULT_LIBRARY_INSTALL_DIR})
