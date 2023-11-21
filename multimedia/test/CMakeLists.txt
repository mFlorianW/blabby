# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
find_package(Qt6Test 6.5 REQUIRED)

###############################################################################
# Includes
###############################################################################
add_subdirectory(dummyplugin)

###############################################################################
# Test Provider
###############################################################################
add_executable(test_multimedia_provider)
target_sources(test_multimedia_provider
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderShould.hpp
)
target_link_libraries(test_multimedia_provider
    Qt6::Test
    Blabby::Multimedia
)
add_test(test_multimedia_provider ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_provider)

###############################################################################
# Test MediaSource
###############################################################################
add_executable(test_multimedia_mediasource)
target_sources(test_multimedia_mediasource
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceShould.hpp
)
target_link_libraries(test_multimedia_mediasource
    Qt6::Test
    Blabby::Multimedia
)
add_test(test_multimedia_mediasource ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_mediasource)

###############################################################################
# Test ProviderLoader
###############################################################################
add_executable(test_multimedia_providerloader)
target_sources(test_multimedia_providerloader
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoaderShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoaderShould.hpp
)
target_link_libraries(test_multimedia_providerloader
    Qt6::Test
    Blabby::Multimedia
    Blabby::DummyPlugin
)
add_test(test_multimedia_providerloader ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_providerloader)

###############################################################################
# Test MediaItem
###############################################################################
add_executable(test_multimedia_mediaitem)
target_sources(test_multimedia_mediaitem
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemShould.hpp
)
target_link_libraries(test_multimedia_mediaitem
    Qt6::Test
    Blabby::Multimedia
)
add_test(test_multimedia_mediaitem ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_mediaitem)
