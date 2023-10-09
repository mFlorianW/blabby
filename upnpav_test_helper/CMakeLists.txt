# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.15)

project(BlabbyUPnPAV
    DESCRIPTION "UPnP/AV implementation for Blabby"
    VERSION 0.2.0
)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Build
###############################################################################
add_library(test_upnp_av_test_helper)
add_library(Blabby::UPnPAVTestHelper ALIAS test_upnp_av_test_helper)

target_sources(test_upnp_av_test_helper
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapMessageTransmitterDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapMessageTransmitterDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSoapCall.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TestSoapCall.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ConnectionManagerActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ConnectionManagerStateVariables.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ContentDirectoryActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ContentDirectoryStateVariables.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Descriptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Descriptions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDiscoveryBackendDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDiscoveryBackendDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptionFetcherBackendDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptionFetcherBackendDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TestableMediaServerProviderFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TestableMediaServerProviderFactory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapCallDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapCallDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ProtocolInfoResponse.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AvTransportActions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AvTransportActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AvTransportStateVariables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AvTransportStateVariables.h
)

target_link_libraries(test_upnp_av_test_helper
    PRIVATE
        Qt6::Test
        Blabby::UPnPAV
)

target_include_directories(test_upnp_av_test_helper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
