# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

###############################################################################
# Subdirectories
###############################################################################
add_subdirectory(doubles)

###############################################################################
# Test MediaServerSource
###############################################################################
add_executable(test_provider_mediaserversource)
target_sources(test_provider_mediaserversource
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSourceShould.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSourceShould.h
)
target_link_libraries(test_provider_mediaserversource
    PRIVATE
        Qt6::Test
        Blabby::MediaServerProvider
        Blabby::MediaServerProviderDoubles
)
add_test(test_provider_mediaserversource ${EXECUTABLE_OUTPUT_PATH}/test_provider_mediaserversource)
