# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

################################################################################
# Build
################################################################################
add_library(test_shell_doubles)
add_library(Blabby::ShellDoubles ALIAS test_shell_doubles)

target_sources(test_shell_doubles
    PRIVATE
        src/TestPlugin.cpp
        src/MemoryPluginSource.cpp
)

target_include_directories(test_shell_doubles
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(test_shell_doubles
    Blabby::PluginCore
    Blabby::Object
)
