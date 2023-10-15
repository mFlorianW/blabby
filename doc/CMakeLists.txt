# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: GPL-2.0-or-later

################################################################################
# Dependencies
################################################################################
find_package(Doxygen REQUIRED)

################################################################################
# Build
################################################################################
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxygen.conf.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxygen.conf)

configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
