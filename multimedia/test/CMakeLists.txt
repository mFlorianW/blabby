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
add_executable(test_multimedia_source)
target_sources(test_multimedia_source
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/SourceShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SourceShould.hpp
)
target_link_libraries(test_multimedia_source
    Qt6::Test
    Blabby::Multimedia
    Blabby::MultimediaTestHelper
)
add_test(test_multimedia_source ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_source)

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
add_executable(test_multimedia_item)
target_sources(test_multimedia_item
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ItemShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ItemShould.hpp
)
target_link_libraries(test_multimedia_item
    Qt6::Test
    Blabby::Multimedia
)
add_test(test_multimedia_item ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_item)

###############################################################################
# Test RendererProvider
###############################################################################
add_executable(test_multimedia_rendererprovider)
target_sources(test_multimedia_rendererprovider
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/RendererProviderShould.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RendererProviderShould.hpp
)
target_link_libraries(test_multimedia_rendererprovider
    Qt6::Test
    Blabby::Multimedia
    Blabby::UPnPAVTestHelper
)
add_test(test_multimedia_rendererprovider ${EXECUTABLE_OUTPUT_PATH}/test_multimedia_rendererprovider)
