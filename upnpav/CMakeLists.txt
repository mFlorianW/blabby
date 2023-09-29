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
find_package(Qt5Xml 5.13 REQUIRED)
find_package(Qt5XmlPatterns 5.13 REQUIRED)

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
    #Service Provider 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceDiscovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/DescriptionFetcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/DeviceDescriptionParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceDiscoveryPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ParsingError.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceControlPointDefinitionParser.cpp
    
    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ScpdStateVariableValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ConnectionManagerServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ServiceDescriptionValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/BaseValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/BaseValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ScpdActionListValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ContentDirectoryServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ServiceValidatorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/SoapMessageGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/HttpSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ErrorResult.cpp
)

set(PRIVATE_HEADERS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceDiscovery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/DescriptionFetcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/DeviceDescriptionParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceDiscoveryPackage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ParsingError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/private/ServiceControlPointDefinitionParser.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/MediaServer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ScpdStateVariableValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ConnectionManagerServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ServiceDescriptionValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ScpdActionListValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ContentDirectoryServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ServiceValidatorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/SoapMessageGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/HttpSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/private/ErrorResult.h
)

set(PUBLIC_SRCS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/DeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/IconDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceControlPointDefinition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDArgument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDStateVariable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/DescriptionFetcherBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceDiscoveryBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceProviderError.cpp

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/SoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/PendingSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/InvalidDeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/SoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/HttpSoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/BrowseResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaServerObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/ProtocolInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/CurrentConnectionIdsResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/CurrentConnectionInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaDevice.cpp
)

set(PUBLIC_HEADERS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/DeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/IconDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceControlPointDefinition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDArgument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/SCPDStateVariable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/DescriptionFetcherBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceDiscoveryBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/IServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/ServiceProviderError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider/IServiceProvider.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaServerObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/BrowseResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/HttpSoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/SoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/InvalidDeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/PendingSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/SoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaServer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/ProtocolInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/CurrentConnectionIdsResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/CurrentConnectionInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device/MediaDevice.h
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
        Qt5::Core
        Qt5::Network
        Qt5::Xml
        Qt5::XmlPatterns
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
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
