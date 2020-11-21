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
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/mediaserver.qrc
)

################################################################################
# Build
################################################################################
add_library(BlabbyMediaServerObject OBJECT)
add_library(Blabby::MediaServer ALIAS BlabbyMediaServerObject)

target_sources(BlabbyMediaServerObject
    PRIVATE
        #Resource
        ${RESOURCE_FILES}

        src/MediaServerPlugin.cpp
)

target_link_libraries(BlabbyMediaServerObject
    Qt5::Core
    Blabby::PluginCore
)

add_library(MediaServer SHARED)

target_link_libraries(MediaServer
    BlabbyMediaServerObject
)

################################################################################
# Install
################################################################################
install(TARGETS MediaServer DESTINATION ${DEFAULT_MULTIMEDIA_PLUGIN_INSTALL_DIR}/MediaServer)