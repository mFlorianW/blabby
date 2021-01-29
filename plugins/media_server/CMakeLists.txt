# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt5_add_resources(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/mediaserver.qrc
)
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resource/qml)

################################################################################
# Build
################################################################################
add_library(BlabbyMediaServerObject OBJECT)
add_library(Blabby::MediaServerObject ALIAS BlabbyMediaServerObject)

target_sources(BlabbyMediaServerObject
    PRIVATE
        #Resource
        ${RESOURCE_FILES}

        src/MediaServerPlugin.cpp
        src/MediaServerModel.cpp
        src/MediaServerView.cpp
        src/ServerItemModel.cpp
        src/ServerItemView.cpp
)

target_link_libraries(BlabbyMediaServerObject
    Qt5::Core
    Qt5::Quick
    Blabby::PluginCore
    Blabby::UPnPAV
)

target_include_directories(BlabbyMediaServerObject
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_library(MediaServer SHARED)

target_link_libraries(MediaServer
    Blabby::MediaServerObject
)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()

################################################################################
# Install
################################################################################
install(TARGETS MediaServer DESTINATION ${DEFAULT_MULTIMEDIA_PLUGIN_INSTALL_DIR}/MediaServer)
