# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Test Browse Result
###############################################################################
add_executable(test_multimedia_provider)
target_sources(test_multimedia_provider
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderShould.h
)
target_link_libraries(test_multimedia_provider
    Qt6::Test
    Blabby::Multimedia
)
add_test(test_multimedia_provider ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_provider)
