# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

################################################################################
# Configure File
################################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ProviderDirs.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/ProviderDirs.hpp)

################################################################################
# QML Modules
################################################################################
add_subdirectory(Blabby/Shell)
add_subdirectory(Blabby/Controls)

################################################################################
# Build blabby
################################################################################
add_library(${PROJECT_NAME}Object SHARED)
add_library(BlabbyShell::Object ALIAS ${PROJECT_NAME}Object)

target_sources(${PROJECT_NAME}Object
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QmlSingletonRegistry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QmlSingletonRegistry.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaPlayer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaPlayer.hpp
)

target_link_libraries(${PROJECT_NAME}Object
    PUBLIC
        Blabby::UPnPAV
        Blabby::Multimedia
        BlabbyShell::Qml
        BlabbyShell::QmlPlugin
        BlabbyShell::Controls
        BlabbyShell::ControlsPlugin
)

target_include_directories(${PROJECT_NAME}Object
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/qml>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_executable(blabby)
target_sources(blabby
    PRIVATE
        main.cpp
)

target_link_libraries(blabby
    PRIVATE
        BlabbyShell::Object
)
################################################################################
# Install
################################################################################
install(TARGETS blabby
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT BlabbyUPnPAV_Runtime)

