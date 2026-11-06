################################################################################
# Project
################################################################################
cmake_minimum_required(VERSION 3.11)

project(BlabbyHttp
    VERSION 0.1.0
    DESCRIPTION "Blabby Http component"
    LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Options
###############################################################################
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN yes)

###############################################################################
# Modules
###############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(CompilerSettings)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LLHTTP REQUIRED libllhttp)

################################################################################
# Subdirectories
################################################################################
add_subdirectory(src)

if(ENABLE_TEST)
    add_subdirectory(test)
endif()
