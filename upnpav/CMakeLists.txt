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
# Options
###############################################################################
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN yes)

###############################################################################
# Modules 
###############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Xml Network)

###############################################################################
# Runtime path
###############################################################################
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
endif("${isSystemDir}" STREQUAL "-1")

################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} OBJECT)
add_library(Blabby::UPnPAV ALIAS ${PROJECT_NAME})
generate_export_header(BlabbyUPnPAV)

set(PRIVATE_SRCS
    # General
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/LoggingCategories.cpp

    #Service Provider 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDiscovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/DescriptionFetcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/DeviceDescriptionParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDiscoveryPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ParsingError.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceControlPointDefinitionParser.cpp
    
    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ScpdStateVariableValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ConnectionManagerServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDescriptionValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/BaseValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/BaseValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ScpdActionListValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ContentDirectoryServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceValidatorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/SoapMessageGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/HttpSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ErrorResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/AvTransportServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ResponseReader.cpp
)

set(PRIVATE_HEADERS
    # General
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/LoggingCategories.h

    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDiscovery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/DescriptionFetcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/DeviceDescriptionParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDiscoveryPackage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ParsingError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceControlPointDefinitionParser.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/MediaServer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ScpdStateVariableValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ConnectionManagerServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceDescriptionValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ScpdActionListValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ContentDirectoryServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ServiceValidatorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/SoapMessageGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/HttpSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ErrorResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/AvTransportServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private/ResponseReader.h
)

set(PUBLIC_SRCS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IconDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceControlPointDefinition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDArgument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDStateVariable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptionFetcherBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDiscoveryBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceProviderError.cpp

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PendingSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InvalidDeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpSoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BrowseResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaServerObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ProtocolInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CurrentConnectionIdsResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CurrentConnectionInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GetMediaInfoResponse.cpp
)

set(PUBLIC_HEADERS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IconDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceControlPointDefinition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDArgument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SCPDStateVariable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptionFetcherBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceDiscoveryBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceProviderError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IServiceProvider.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaServerObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BrowseResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HttpSoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InvalidDeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PendingSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaServer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ProtocolInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CurrentConnectionIdsResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CurrentConnectionInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MediaDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GetMediaInfoResponse.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PRIVATE_SRCS}
        ${PRIVATE_HEADERS}
        ${PUBLIC_SRCS}
        ${PUBLIC_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Network
        Qt6::Xml
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/device>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# CMakePackageConfig
################################################################################
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header blabby UPNPAV files" )

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/BlabbyUPnPAVConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BlabbyUPnPAV
    PATH_VARS INCLUDE_INSTALL_DIR
)

write_basic_package_version_file(
    ${PROJECT_NAME}Version.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion
)

################################################################################
# Install
################################################################################
install(TARGETS BlabbyUPnPAV EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT BlabbyUPnPAV_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT BlabbyUPnPAV_Development
    NAMELINK_COMPONENT BlabbyUPnPAV_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${PUBLIC_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}")
install(FILES ${PRIVATE_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}/private")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)


################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()
