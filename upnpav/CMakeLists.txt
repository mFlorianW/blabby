# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

cmake_minimum_required(VERSION 3.11)

project(LibBlabbyUPnPAV
    DESCRIPTION "UPnP/AV implementation for Blabby"
    VERSION 0.2.0
)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt5Xml 5.13 REQUIRED)
find_package(Qt5XmlPatterns 5.13 REQUIRED)

###############################################################################
# Runtime path
###############################################################################
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}")
endif("${isSystemDir}" STREQUAL "-1")

################################################################################
# Build
################################################################################
add_definitions(-DBUILDING_UPNP)

add_library(BlabbyUPnPAVObject OBJECT)
add_library(Blabby::UPnPAVObject ALIAS BlabbyUPnPAVObject)
target_sources(BlabbyUPnPAVObject
    PRIVATE
        #Service Provider
        src/service_provider/ServiceProvider.cpp
        src/service_provider/DeviceDescription.cpp
        src/service_provider/IconDescription.cpp
        src/service_provider/ServiceDescription.cpp
        src/service_provider/ServiceControlPointDefinition.cpp
        src/service_provider/SCPDAction.cpp
        src/service_provider/SCPDArgument.cpp
        src/service_provider/SCPDStateVariable.cpp
        src/service_provider/ServiceDiscoveryBackend.cpp
        src/service_provider/ServiceDiscovery.cpp
        src/service_provider/DescriptionFetcher.cpp
        src/service_provider/DescriptionFetcherBackend.cpp
        src/service_provider/DeviceDescriptionParser.cpp
        src/service_provider/ServiceDiscoveryPackage.cpp
        src/service_provider/ParsingError.cpp
        src/service_provider/ServiceControlPointDefinitionParser.cpp
        src/service_provider/ServiceProviderError.cpp

        #device
        src/device/InvalidDeviceDescription.cpp
        src/device/MediaServer.cpp
        src/device/ScpdStateVariableValidator.cpp
        src/device/ConnectionManagerServiceValidator.cpp
        src/device/ServiceDescriptionValidator.cpp
        src/device/BaseValidator.cpp
        src/device/ScpdActionListValidator.cpp
        src/device/ContentDirectoryServiceValidator.cpp
        src/device/ServiceValidatorBase.cpp
        src/device/SoapMessageTransmitter.cpp
        src/device/SoapMessageGenerator.cpp
        src/device/SoapCall.cpp
        src/device/PendingSoapCall.cpp
        src/device/HttpSoapMessageTransmitter.cpp
        src/device/HttpSoapCall.cpp
        src/device/ErrorResult.cpp
        src/device/BrowseResult.cpp
        src/device/MediaServerObject.cpp

        #Public Header must be add manualy otherwise vtable
        #errors occure.
        include/UPnP_Export.h
        #service provider
        include/service_provider/ServiceProvider.h
        include/service_provider/DeviceDescription.h
        include/service_provider/IconDescription.h
        include/service_provider/ServiceDescription.h
        include/service_provider/ServiceControlPointDefinition.h
        include/service_provider/SCPDAction.h
        include/service_provider/SCPDArgument.h
        include/service_provider/SCPDStateVariable.h
        include/service_provider/DescriptionFetcherBackend.h
        include/service_provider/ServiceDiscoveryBackend.h
        include/service_provider/IServiceProvider.h
        include/service_provider/ServiceProviderError.h
        include/service_provider/IServiceProvider.h

        #device
        include/device/MediaServer.h
        include/device/SoapCall.h
        include/device/PendingSoapCall.h
        include/device/InvalidDeviceDescription.h
        include/device/SoapMessageTransmitter.h
        include/device/HttpSoapMessageTransmitter.h
        include/device/BrowseResult.h
        include/device/MediaServerObject.h
        include/device/IMediaServer.h
)

target_link_libraries(BlabbyUPnPAVObject
    PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::Xml
        Qt5::XmlPatterns
)

target_include_directories(BlabbyUPnPAVObject
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/service_provider>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/device>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider
)

add_library(BlabbyUPnPAV SHARED)
add_library(Blabby::UPnPAV ALIAS BlabbyUPnPAV)

target_link_libraries(BlabbyUPnPAV
    BlabbyUPnPAVObject
)

set_target_properties(BlabbyUPnPAV PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()

################################################################################
# Install
################################################################################
INSTALL(TARGETS BlabbyUPnPAV
    DESTINATION ${DEFAULT_LIBRARY_INSTALL_DIR})

INSTALL(DIRECTORY include/
    DESTINATION ${DEFAULT_INCLUDE_INSTALL_DIR}/BlabbyUPnPAV)
