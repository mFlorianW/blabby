# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

###############################################################################
# Build
###############################################################################
add_library(test_upnp_av_test_helper)
add_library(Blabby::UPnPAVTestHelper ALIAS test_upnp_av_test_helper)

target_sources(test_upnp_av_test_helper
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/SoapMessageTransmitterDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SoapMessageTransmitterDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TestSoapCall.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestSoapCall.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionManagerActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionManagerStateVariables.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ContentDirectoryActions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ContentDirectoryActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ContentDirectoryStateVariables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ContentDirectoryStateVariables.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Descriptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Descriptions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDiscoveryBackendDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDiscoveryBackendDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionFetcherBackendDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionFetcherBackendDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/TestableMediaServerProviderFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestableMediaServerProviderFactory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SoapCallDouble.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SoapCallDouble.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ProtocolInfoResponse.h
        ${CMAKE_CURRENT_SOURCE_DIR}/AvTransportActions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AvTransportActions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/AvTransportStateVariables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AvTransportStateVariables.h
)

target_link_libraries(test_upnp_av_test_helper
    PRIVATE
        Qt6::Test
        Blabby::UPnPAV
)

target_include_directories(test_upnp_av_test_helper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
