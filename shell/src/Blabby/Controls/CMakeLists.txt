# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later
################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}Controls STATIC
    URI
        "Blabby.Controls"
    VERSION
        1.0
    QML_FILES
        Clock.qml
        IconButton.qml
        ListEntry.qml
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractIconButton.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractIconButton.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractListEntry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractListEntry.hpp
)
add_library(BlabbyShell::Controls ALIAS ${PROJECT_NAME}Controls)
add_library(BlabbyShell::ControlsPlugin ALIAS ${PROJECT_NAME}Controlsplugin)

target_link_libraries(${PROJECT_NAME}Controls
    PUBLIC
        Qt6::Quick
)
