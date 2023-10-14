# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} OBJECT)
add_library(Blabby::UPnPAV ALIAS ${PROJECT_NAME})
generate_export_header(BlabbyUPnPAV)

set(PRIVATE_SRCS
    # General
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp

    #Service Provider 
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscovery.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DescriptionFetcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DeviceDescriptionParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscoveryPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ParsingError.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceControlPointDefinitionParser.cpp
    
    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdStateVariableValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ConnectionManagerServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDescriptionValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/BaseValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/BaseValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdActionListValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ContentDirectoryServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceValidatorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/SoapMessageGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/HttpSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ErrorResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/AvTransportServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseReader.cpp
)

set(PRIVATE_HEADERS
    # General
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.h

    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscovery.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DescriptionFetcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DeviceDescriptionParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscoveryPackage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ParsingError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceControlPointDefinitionParser.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/private/MediaServer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdStateVariableValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ConnectionManagerServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDescriptionValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdActionListValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ContentDirectoryServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceValidatorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/SoapMessageGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/HttpSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ErrorResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/AvTransportServiceValidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseReader.h
)

set(PUBLIC_SRCS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IconDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceControlPointDefinition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDArgument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDStateVariable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionFetcherBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDiscoveryBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderError.cpp

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PendingSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InvalidDeviceDescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpSoapMessageTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BrowseResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProtocolInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionIdsResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionInfoResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetMediaInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportInfoResponse.cpp
)

set(PUBLIC_HEADERS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IconDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceControlPointDefinition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDArgument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDStateVariable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionFetcherBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDiscoveryBackend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IServiceProvider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderError.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IServiceProvider.h

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BrowseResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpSoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapMessageTransmitter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/InvalidDeviceDescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PendingSoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapCall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ProtocolInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionIdsResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurrentConnectionInfoResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GetMediaInfoResponse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportInfoResponse.h
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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
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
