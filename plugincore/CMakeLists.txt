#
# This file is part of the Blabby project.
# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################
# Project
###############################################################################
cmake_minimum_required(VERSION 3.11)

project(LibBlabbyPluginCore
    DESCRIPTION "Plugin Core of the Blabby project"
    VERSION 0.1.0
)

################################################################################
# Resource files
################################################################################
qt5_add_resources(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.qrc
)
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resource/qml)

################################################################################
# Definitions
################################################################################
add_definitions(-DPLUGINCORE)

################################################################################
# Build PluginCore
################################################################################
add_library(BlabbyPluginCore SHARED)
add_library(Blabby::PluginCore ALIAS BlabbyPluginCore)

target_sources(BlabbyPluginCore
    PRIVATE
        ${RESOURCE_FILES}
        src/MultimediaPlugin.cpp

    #Public header
        include/PluginCore_Export.h
        include/MultimediaPlugin.h
        include/PluginVersion.h
)

target_link_libraries(BlabbyPluginCore
    Qt5::Core
)

target_include_directories(BlabbyPluginCore
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(BlabbyPluginCore PROPERTIES
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
# Install plugin
################################################################################
install(TARGETS BlabbyPluginCore DESTINATION ${DEFAULT_LIBRARY_INSTALL_DIR})
install(DIRECTORY include/ DESTINATION ${DEFAULT_INCLUDE_INSTALL_DIR}/BlabbyPluginCore)
