# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later

###############################################################################
# Build Shell Test Doules
###############################################################################
add_library(BlabbyShellTestDoubles STATIC)
add_library(BlabbyShell::TestDoubles ALIAS BlabbyShellTestDoubles)

target_sources(BlabbyShellTestDoubles
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProviderLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProviderLoader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestMediaSource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TestMediaSource.hpp
)

target_link_libraries(BlabbyShellTestDoubles
    Qt6::Core
    BlabbyShell::Object
    Blabby::Multimedia
)

target_include_directories(BlabbyShellTestDoubles
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
