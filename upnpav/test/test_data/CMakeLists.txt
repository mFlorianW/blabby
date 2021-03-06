# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(test_upnp_av_test_data)
add_library(Blabby::UPnPAVTestData ALIAS test_upnp_av_test_data)

target_sources(test_upnp_av_test_data
    PRIVATE
        src/SoapMessageTransmitterDouble.cpp
        src/TestSoapCall.cpp
        src/MediaDeviceBaseTest.cpp

        include/TestSoapCall.h
        include/MediaDeviceBaseTest.h
)

target_link_libraries(test_upnp_av_test_data
    Qt5::Test
)

target_include_directories(test_upnp_av_test_data
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(test_upnp_av_test_data
    Blabby::UPnPAVObject
)
