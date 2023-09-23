# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(test_media_server_doubles)
add_library(Blabby::MediaServerPluginDoubles ALIAS test_media_server_doubles)

target_sources(test_media_server_doubles
    PRIVATE
        src/MediaServerDouble.cpp
        src/ServiceProviderDouble.cpp
        src/SoapCallDouble.cpp
)

target_include_directories(test_media_server_doubles
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(test_media_server_doubles
    Blabby::UPnPAV
)
