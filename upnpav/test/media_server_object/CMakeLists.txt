# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

add_executable(test_upnpav_media_server_object)

target_sources(test_upnpav_media_server_object
    PRIVATE
        src/MediaServerObjectShould.cpp
)

target_include_directories(test_upnpav_media_server_object
    PRIVATE
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/data>
)

target_link_libraries(test_upnpav_media_server_object
    Blabby::UPnPAVObject
    Qt5::Test
)

add_test(test_upnpav_browse_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_media_server_object)
