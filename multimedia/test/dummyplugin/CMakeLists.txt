# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
###############################################################################
# DummyPlugin
###############################################################################
set(DUMMY_PLUGIN_FOLDER ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/DummyPluginFolder.h.in ${CMAKE_CURRENT_BINARY_DIR}/DummyPluginFolder.h @ONLY)

add_library(DummyPlugin SHARED)
add_library(Blabby::DummyPlugin ALIAS DummyPlugin)
target_sources(DummyPlugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/DummyPlugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DummyPlugin.h
)
target_link_libraries(DummyPlugin
    Blabby::Multimedia
)
target_include_directories(DummyPlugin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
