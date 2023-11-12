# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

################################################################################
# Resource files
################################################################################
qt_add_resources(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/resource.qrc
)

###############################################################################
# Runtime path
###############################################################################
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

################################################################################
# Build blabby
################################################################################
add_library(BlabbyObject OBJECT)
add_library(Blabby::Object ALIAS BlabbyObject)

target_sources(BlabbyObject
    PRIVATE
        ${RESOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Clock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Clock.h
)

target_link_libraries(BlabbyObject
    PUBLIC
        Blabby::UPnPAV
        Blabby::Multimedia
        Qt6::Quick
)

target_include_directories(BlabbyObject
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_executable(blabby)
target_sources(blabby
    PRIVATE
        src/main.cpp
)

target_link_libraries(blabby
    PRIVATE
        BlabbyObject
)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    # add_subdirectory(test)
endif()

################################################################################
# Install FreeDLNAControl
################################################################################
INSTALL(TARGETS blabby DESTINATION bin)
