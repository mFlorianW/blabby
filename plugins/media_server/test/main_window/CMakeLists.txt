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
add_executable(test_media_server_plugin_main_window)

target_sources(test_media_server_plugin_main_window
    PRIVATE
        src/MainWindowShould.cpp
)

#target_include_directories(test_media_server_plugin_media_server_model
#)

target_link_libraries(test_media_server_plugin_main_window
    Blabby::MediaServerObject
    Blabby::MediaServerPluginDoubles
    Qt5::Test
)

add_test(test_media_server_plugin_main_controller ${EXECUTABLE_OUTPUT_PATH}/test_media_server_plugin_main_window)
