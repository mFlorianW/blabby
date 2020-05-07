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

find_package(Qt5Test 5.13 REQUIRED)

################################################################################
# Resource files
################################################################################
qt5_add_resources(TEST_RESOURCE_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/upnpav.qrc
)

################################################################################
# Build
################################################################################
add_executable(test_upnpav)

target_sources(test_upnpav
    PRIVATE
        ${TEST_RESOURCE_FILE}
        src/main.cpp
        src/service_provider/ServiceProviderShould.cpp
        src/service_provider/doubles/ServiceDiscoveryBackendDouble.cpp
        src/service_provider/doubles/DescriptionFetcherBackendDouble.cpp
        src/service_provider/doubles/TestableMediaServerProviderFactory.cpp

)

target_link_libraries(test_upnpav
    Blabby::UPnPAVObject
    Qt5::Test
)

target_include_directories(test_upnpav
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/service_provider>
)

set_property(TARGET test_upnpav PROPERTY UPnPAV_PRIVATE 1)

add_test(test_shell ${PROJECT_BINARY_DIR}/test_upnpav)
