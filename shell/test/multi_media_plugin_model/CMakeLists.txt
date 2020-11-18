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
add_executable(test_shell_multimedia_plugin_model)

target_sources(test_shell_multimedia_plugin_model
    PRIVATE
        src/MultiMediaPluginModelShould.cpp
)

target_include_directories(test_shell_multimedia_plugin_model
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/double>
)

target_link_libraries(test_shell_multimedia_plugin_model
    Blabby::Object
    Qt5::Test
)

add_test(test_shell_multimedia_plugin_model
    ${EXECUTABLE_OUTPUT_PATH}/test_shell_multimedia_plugin_model
)
