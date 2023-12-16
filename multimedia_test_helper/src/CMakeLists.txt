# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

###############################################################################
# Build Shell Test Doules
###############################################################################
add_library(BlabbyMultimediaTestHelper STATIC)
add_library(Blabby::MultimediaTestHelper ALIAS BlabbyMultimediaTestHelper)

target_sources(BlabbyMultimediaTestHelper
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProviderLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProviderLoader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestSource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestSource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.hpp
)

target_link_libraries(BlabbyMultimediaTestHelper
    Qt6::Core
    Blabby::Multimedia
)

target_include_directories(BlabbyMultimediaTestHelper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
