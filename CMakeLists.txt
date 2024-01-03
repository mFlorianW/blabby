cmake_minimum_required(VERSION 3.22)

project(Blabby
    VERSION 0.1.0
    DESCRIPTION "Blabby UPnP control point for Raspberry PI"
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/configuration/cmake/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# Modules
################################################################################
include(GenerateExportHeader)
include(FeatureSummary)
include(PreCommit)

################################################################################
# Options
################################################################################
enable_testing()
option(ENABLE_TEST "Enable/Disable tests build. Default: false" OFF)
add_feature_info(EnableTest ENABLE_TEST "Test building enabled.")
option(BUILD_DOC "Enable/Disable documentation build. Default: false" OFF)
add_feature_info(BuildDoc BUILD_DOC "Documentation build.")
option(ENABLE_PRECOMMIT "Enables automatic pre-commit install." OFF)
add_feature_info(PreCommit ENABLE_PRECOMMIT "Automatic install of the Python pre-commit tool.")

################################################################################
# PreCommit
################################################################################
if(ENABLE_PRECOMMIT)
    setup_precommit()
endif()

################################################################################
# Variables
################################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BASE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

################################################################################
# Subdirectories
################################################################################
add_subdirectory(upnpav)
add_subdirectory(shell)
add_subdirectory(providers)
add_subdirectory(multimedia)

if (ENABLE_TEST)
    add_subdirectory(upnpav_test_helper)
    add_subdirectory(multimedia_test_helper)
endif()

if(BUILD_DOC)
    add_subdirectory(docs)
endif()

feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
