# Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

###############################################################################
# Tests
###############################################################################
add_executable(test_plugin_core_multimedia_plugin)

target_sources(test_plugin_core_multimedia_plugin
    PRIVATE
        MultimediaPluginShould.cpp
        MultimediaPluginShould.h
)

#target_include_directories(test_media_server_plugin_media_server_model
#)

target_link_libraries(test_plugin_core_multimedia_plugin
    Blabby::PluginCore
    Qt6::Test
)

add_test(test_plugin_core_multimedia_plugin ${EXECUTABLE_OUTPUT_PATH}/test_plugin_core_multimedia_plugin)
