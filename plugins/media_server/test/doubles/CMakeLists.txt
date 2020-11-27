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
# Build
################################################################################
add_library(test_media_server_doubles)
add_library(Blabby::MediaServerPluginDoubles ALIAS test_media_server_doubles)

target_sources(test_media_server_doubles
    PRIVATE
        src/MediaServerDouble.cpp
        src/ServiceProviderDouble.cpp
)

target_include_directories(test_media_server_doubles
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(test_media_server_doubles
    Blabby::UPnPAV
)
