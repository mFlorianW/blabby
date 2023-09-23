# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_executable(test_media_server_plugin_server_item_view)

target_sources(test_media_server_plugin_server_item_view
    PRIVATE
        src/ServerItemViewShould.cpp
)

target_include_directories(test_media_server_plugin_server_item_view
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/data>
)

target_link_libraries(test_media_server_plugin_server_item_view
    Blabby::MediaServerObject
    Blabby::MediaServerPluginDoubles
    Qt5::Test
)

add_test(test_media_server_plugin_server_item_view ${EXECUTABLE_OUTPUT_PATH}/test_media_server_plugin_server_item_view)
