# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_executable(test_shell_multimedia_plugin_model)

target_sources(test_shell_multimedia_plugin_model
    PRIVATE
        src/MultiMediaPluginModelShould.cpp
)

target_include_directories(test_shell_multimedia_plugin_model
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/double>
)

target_link_libraries(test_shell_multimedia_plugin_model
    Blabby::Object
    Blabby::ShellDoubles
    Qt5::Test
)

add_test(test_shell_multimedia_plugin_model
    ${EXECUTABLE_OUTPUT_PATH}/test_shell_multimedia_plugin_model
)
