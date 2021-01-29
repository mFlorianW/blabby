# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

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
