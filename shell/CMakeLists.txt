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
################################################################################
# Configure header files
################################################################################
configure_file(src/main_window/src/PluginDirs.h.in ${CMAKE_CURRENT_BINARY_DIR}/PluginDirs.h)

################################################################################
# Resource files
################################################################################
qt5_add_resources(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.qrc
)

###############################################################################
# Runtime path
###############################################################################
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

################################################################################
# Build opendvb
################################################################################
add_library(BlabbyObject OBJECT)
add_library(Blabby::Object ALIAS BlabbyObject)

target_sources(BlabbyObject
    PRIVATE
        ${RESOURCE_FILES}
        # Logging
        src/LoggingCategories.cpp

        # Plugin
        src/plugin/src/MultiMediaPluginModel.cpp
        src/plugin/src/FileSystemPluginSource.cpp

        # MainWindow
        src/main_window/src/MainWindow.cpp
        src/main_window/src/Clock.cpp

        #Public Includes
        # Plugin
        src/plugin/include/MultiMediaPluginSource.h
        src/plugin/include/MultiMediaPluginModel.h
        src/plugin/include/FileSystemPluginSource.h

        # MainWindow
        src/main_window/include/MainWindow.h
        src/main_window/include/Clock.h
)

target_link_libraries(BlabbyObject
    PUBLIC
        Blabby::UPnPAV
        Blabby::PluginCore
        Qt5::Quick
)

target_include_directories(BlabbyObject
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/plugin/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main_window/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_executable(blabby)
target_sources(blabby
    PRIVATE
        src/main.cpp
)

target_link_libraries(blabby
    BlabbyObject
)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()

################################################################################
# Install FreeDLNAControl
################################################################################
INSTALL(TARGETS blabby DESTINATION bin)
