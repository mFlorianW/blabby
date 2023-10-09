# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_executable(test_media_server_plugin_server_item_model)

target_sources(test_media_server_plugin_server_item_model
    PRIVATE
        src/ServerItemModelShould.cpp
)

#target_include_directories(test_media_server_plugin_media_server_model
#)

target_link_libraries(test_media_server_plugin_server_item_model
    Blabby::MediaServerObject
    Blabby::MediaServerPluginDoubles
    Qt6::Test
)

add_test(test_media_server_plugin_server_item_model ${EXECUTABLE_OUTPUT_PATH}/test_media_server_plugin_server_item_model)
