# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

add_executable(test_upnpav_media_device)
target_sources(test_upnpav_media_device
    PRIVATE
        src/MediaDeviceShould.cpp
)

target_include_directories(test_upnpav_media_server
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/data>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/double>
)

target_link_libraries(test_upnpav_media_device
    Blabby::UPnPAVObject
    Blabby::UPnPAVTestData
    Qt5::Test
)

add_test(test_upnpav_media_server ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_media_device)
