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
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}")
ENDIF("${isSystemDir}" STREQUAL "-1")

################################################################################
# Build opendvb
################################################################################
add_library(BlabbyObject OBJECT)
add_library(Blabby::Object ALIAS BlabbyObject)

target_sources(BlabbyObject
    PRIVATE
        ${RESOURCE_FILES}
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
# Install FreeDLNAControl
################################################################################
INSTALL(TARGETS blabby DESTINATION bin)
