# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(MediaServerProviderDoubles)
add_library(Blabby::MediaServerProviderDoubles ALIAS MediaServerProviderDoubles)

target_sources(MediaServerProviderDoubles
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Response.h
)

target_include_directories(MediaServerProviderDoubles
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(MediaServerProviderDoubles
    Blabby::UPnPAV
    Blabby::UPnPAVTestHelper
)
