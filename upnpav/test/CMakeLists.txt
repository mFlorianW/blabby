# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test Browse Result 
###############################################################################
add_executable(test_upnpav_browse_result)
target_sources(test_upnpav_browse_result
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/BrowseResultShould.cpp
)
target_include_directories(test_upnpav_browse_result
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_upnpav_browse_result
    Qt6::Test
    Blabby::UPnPAV
    Blabby::UPnPAVTestHelper
)
add_test(test_upnpav_browse_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_browse_result)

###############################################################################
# Test Media Server
###############################################################################
add_executable(test_upnpav_media_server)
target_sources(test_upnpav_media_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerShould.cpp
)
target_link_libraries(test_upnpav_media_server
    PUBLIC 
        Blabby::UPnPAV
        Blabby::UPnPAVTestHelper
        Qt6::Test
)
add_test(test_upnpav_media_server ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_media_server)

###############################################################################
# Test Media Server Object
###############################################################################
add_executable(test_upnpav_media_server_object)
target_sources(test_upnpav_media_server_object
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerObjectShould.cpp
)
target_include_directories(test_upnpav_media_server_object
    PRIVATE
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_upnpav_media_server_object
    Blabby::UPnPAV
    Qt6::Test
)
add_test(test_upnpav_browse_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_media_server_object)

###############################################################################
# Test Pending SoapCall
###############################################################################
add_executable(test_upnpav_pending_soap_call)
target_sources(test_upnpav_pending_soap_call
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/PendingSoapCallShould.cpp
)
target_include_directories(test_upnpav_pending_soap_call
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data>
)
target_link_libraries(test_upnpav_pending_soap_call
    Blabby::UPnPAV
    Blabby::UPnPAVTestHelper
    Qt6::Test
)
add_test(test_upnpav_pending_soap_call ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_pending_soap_call)

################################################################################
# Test Service Provider
################################################################################
add_executable(test_upnpav_service_provider)
qt_add_resources(TEST_RESOURCE_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/upnpav.qrc
)
target_sources(test_upnpav_service_provider
    PRIVATE
        ${TEST_RESOURCE_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderShould.cpp
)
target_link_libraries(test_upnpav_service_provider
    PRIVATE
        Blabby::UPnPAV
        Qt6::Test
        Blabby::UPnPAVTestHelper
)
add_test(test_upnpav_service_provider ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_service_provider)

################################################################################
# Test Protocol Info Result
################################################################################
add_executable(test_upnpav_protocol_info_result)
target_sources(test_upnpav_protocol_info_result
    PRIVATE
        ${TEST_RESOURCE_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/ProtocolInfoResultShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProtocolInfoResultShould.h
)
target_link_libraries(test_upnpav_protocol_info_result
    PRIVATE
        Blabby::UPnPAV
        Blabby::UPnPAVTestHelper
        Qt6::Test
)
add_test(test_upnpav_protocol_info_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_protocol_info_result)

################################################################################
# Test Current Connections Ids Reult
################################################################################
add_executable(test_upnpav_current_connection_ids_result)
target_sources(test_upnpav_current_connection_ids_result
    PRIVATE
        ${TEST_RESOURCE_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionIdsResultShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionIdsResultShould.h
)
target_link_libraries(test_upnpav_current_connection_ids_result
    PRIVATE
        Blabby::UPnPAV
        Blabby::UPnPAVTestHelper
        Qt6::Test
)
add_test(test_upnpav_current_connection_ids_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_current_connection_ids_result)

################################################################################
# Test Current Connection Info Reult
################################################################################
add_executable(test_upnpav_current_connection_info_result)
target_sources(test_upnpav_current_connection_info_result
    PRIVATE
        ${TEST_RESOURCE_FILE}
        ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionInfoResultShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionInfoResultShould.h
)
target_link_libraries(test_upnpav_current_connection_info_result
    PRIVATE
        Blabby::UPnPAV
        Blabby::UPnPAVTestHelper
        Qt6::Test
)
add_test(test_upnpav_current_connection_info_result ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_current_connection_info_result)

###############################################################################
# Test Media Device
###############################################################################
add_executable(test_upnpav_media_device)
target_sources(test_upnpav_media_device
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaDeviceShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaDeviceShould.h
)
target_link_libraries(test_upnpav_media_device
    PUBLIC
        Blabby::UPnPAV
        Blabby::UPnPAVTestHelper
        Qt6::Test
)
add_test(test_upnpav_media_device ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_media_device)
