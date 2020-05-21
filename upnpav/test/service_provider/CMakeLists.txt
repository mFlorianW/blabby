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
################################################################################
# Resource files
################################################################################
qt5_add_resources(TEST_RESOURCE_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/upnpav.qrc
)

################################################################################
# Build
################################################################################
add_executable(test_upnpav_service_provider)

target_sources(test_upnpav_service_provider
    PRIVATE
        ${TEST_RESOURCE_FILE}
        src/ServiceProviderShould.cpp
        src/doubles/ServiceDiscoveryBackendDouble.cpp
        src/doubles/DescriptionFetcherBackendDouble.cpp
        src/doubles/TestableMediaServerProviderFactory.cpp
)

target_link_libraries(test_upnpav_service_provider
    Blabby::UPnPAVObject
    Qt5::Test
)

add_test(test_upnpav_service_provider ${EXECUTABLE_OUTPUT_PATH}/test_upnpav_service_provider)
