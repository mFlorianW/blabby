cmake_minimum_required(VERSION 3.15)

project(BlabbyMultiMediaTestHelper
    DESCRIPTION "Static library with test Helper for the Multimedia Library."
    VERSION 0.1.0
)

###############################################################################
# Modules
###############################################################################
include(CompilerSettings)

###############################################################################
# Dependencies
###############################################################################
find_package(Qt6Core 6.5 REQUIRED)

###############################################################################
# Source
###############################################################################
add_subdirectory(src)
