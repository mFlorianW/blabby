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
# Project settings
################################################################################
cmake_minimum_required(VERSION 3.11)

project(Blabby
    VERSION 0.1.0
    DESCRIPTION "Blabby UPnP control point for Raspberry PI"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

################################################################################
# Options
################################################################################
enable_testing()
option(ENABLE_TEST "Enable/Disable tests build. Default: false" OFF)

################################################################################
# Variables
################################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BASE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/controls ${CMAKE_BINARY_DIR} CACHE STRING "" FORCE)

#Install folder
set(DEFAULT_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
set(DEFAULT_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})
set(DEFAULT_MULTIMEDIA_PLUGIN_INSTALL_DIR ${DEFAULT_LIBRARY_INSTALL_DIR}/plugins/multimedia)
set(DEFAULT_QML_PLUGIN_INSTALL_DIR ${DEFAULT_LIBRARY_INSTALL_DIR}/plugins/qml)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

################################################################################
# Required Packages
################################################################################
find_package(Qt5Core 5.13 REQUIRED)
find_package(Qt5Quick 5.13 REQUIRED)

################################################################################
# Compiler-Settings
################################################################################
if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g3 -fno-inline)

        if(ENABLE_TEST)
            #add_compile_options(-fprofile-arcs -ftest-coverage)
        endif()
    else()
        add_compile_options(-O2)
    endif()

    add_compile_options(-Wall -Wextra -Wunused-function -fvisibility=hidden -fmessage-length=0)
endif()

add_definitions("-DQT_NO_KEYWORDS")
add_definitions("-DINSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

################################################################################
# Subdirectories
################################################################################
add_subdirectory(upnpav)
