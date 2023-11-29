# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
###############################################################################
# Dependencies
###############################################################################
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test MediaSourceModel
###############################################################################
add_executable(test_shell_mediasource_model)
target_sources(test_shell_mediasource_model
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModelShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModelShould.hpp
)
target_include_directories(test_shell_mediasource_model
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_shell_mediasource_model
    PRIVATE
        Qt6::Test
        BlabbyShell::Object
        Blabby::MultimediaTestHelper
)
add_test(test_shell_mediasource_model ${EXECUTABLE_OUTPUT_PATH}/test_shell_mediasource_model)
