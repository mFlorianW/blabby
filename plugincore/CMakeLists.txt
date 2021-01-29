# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.11)

project(LibBlabbyPluginCore
    DESCRIPTION "Plugin Core of the Blabby project"
    VERSION 0.1.0
)

################################################################################
# Definitions
################################################################################
add_definitions(-DPLUGINCORE)

################################################################################
# Build PluginCore
################################################################################
add_library(BlabbyPluginCore SHARED)
add_library(Blabby::PluginCore ALIAS BlabbyPluginCore)

target_sources(BlabbyPluginCore
    PRIVATE
        src/MultimediaPlugin.cpp

    #Public header
        include/PluginCore_Export.h
        include/MultimediaPlugin.h
        include/PluginVersion.h
)

target_link_libraries(BlabbyPluginCore
    Qt5::Core
)

target_include_directories(BlabbyPluginCore
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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
install(DIRECTORY include/ DESTINATION ${DEFAULT_INCLUDE_INSTALL_DIR}/BlabbyPluginCore)
