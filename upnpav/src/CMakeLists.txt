# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} SHARED)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/private/BaseValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdActionListValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ContentDirectoryServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceValidatorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/SoapMessageGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/HttpSoapCall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ErrorResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/AvTransportServiceValidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/RenderingControlServiceValidator.cpp
)

set(PRIVATE_HEADERS
    # General
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp

    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscovery.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DescriptionFetcher.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/DeviceDescriptionParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDiscoveryPackage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ParsingError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceControlPointDefinitionParser.hpp

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/private/MediaServer_p.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdStateVariableValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ConnectionManagerServiceValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceDescriptionValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ScpdActionListValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ContentDirectoryServiceValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServiceValidatorBase.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/SoapMessageGenerator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/HttpSoapCall.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ErrorResult.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/AvTransportServiceValidator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseReader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/RenderingControlServiceValidator.hpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/BrowseResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetProtocolInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetCurrentConnectionIdsResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetCurrentConnectionInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetMediaInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetPositionInfoResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetDeviceCapabilitiesResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportSettingsResponse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaRenderer.cpp
)

set(PUBLIC_HEADERS
    #Service Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProvider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeviceDescription.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IconDescription.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDescription.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceControlPointDefinition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDAction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDArgument.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SCPDStateVariable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionFetcherBackend.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceDiscoveryBackend.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IServiceProvider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceProviderError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IServiceProvider.hpp

    #Device
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerObject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BrowseResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpSoapMessageTransmitter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapMessageTransmitter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InvalidDeviceDescription.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PendingSoapCall.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SoapCall.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaServer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetProtocolInfoResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetCurrentConnectionIdsResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetCurrentConnectionInfoResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaDevice.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetMediaInfoResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportInfoResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetPositionInfoResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetDeviceCapabilitiesResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GetTransportSettingsResponse.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaRenderer.hpp
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
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
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
