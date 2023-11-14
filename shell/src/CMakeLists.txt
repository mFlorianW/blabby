# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

################################################################################
# Resource files
################################################################################
qt_add_resources(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.qrc
)

################################################################################
# Build blabby
################################################################################
add_library(${PROJECT_NAME}Object SHARED)
add_library(BlabbyShell::Object ALIAS ${PROJECT_NAME}Object)

target_sources(${PROJECT_NAME}Object
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.h
)

target_link_libraries(${PROJECT_NAME}Object
    PUBLIC
        Blabby::UPnPAV
        Blabby::Multimedia
        Qt6::Quick
)

target_include_directories(${PROJECT_NAME}Object
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/qml>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        BlabbyShellObject
)
################################################################################
# QML Module
################################################################################
set_source_files_properties(qml/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}Object
    URI
        "${PROJECT_NAME}"
    VERSION
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    QML_FILES
        qml/MainWindow.qml
        qml/Clock.qml
        qml/Theme.qml
)

################################################################################
# Install
################################################################################
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT BlabbyUPnPAV_Runtime)

