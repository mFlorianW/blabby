cmake_minimum_required(VERSION 3.15)

project(BlabbyUPnPAV
    DESCRIPTION "UPnP/AV implementation for Blabby"
    VERSION 0.2.0
)

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
# Runtime path
###############################################################################
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
endif("${isSystemDir}" STREQUAL "-1")

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6 6.5 REQUIRED COMPONENTS Xml Network)

################################################################################
# Source
################################################################################
add_subdirectory(src)

################################################################################
# Test
################################################################################
if(ENABLE_TEST)
    add_subdirectory(test)
endif()
