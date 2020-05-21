##
## This file is part of the Blabby.
## Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as
## published by the Free Software Foundation, either version 2 of the
## License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
###############################################################################
# Project
###############################################################################
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

        #device
        src/device/InvalidDeviceDescription.cpp
        src/device/MediaServer.cpp
        src/device/ScpdStateVariableValidator.cpp

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

        #device
        include/device/MediaServer.h
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
