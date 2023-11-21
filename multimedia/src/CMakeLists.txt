# Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
# SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
#
# SPDX-License-Identifier: LGPL-2.1-or-later
################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} SHARED)
add_library(Blabby::Multimedia ALIAS ${PROJECT_NAME})
generate_export_header(${PROJECT_NAME})

set(PRIVATE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp
)

set(PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp
)

set(PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Provider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaItem.cpp
)

set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Provider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaSource.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MediaItem.hpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PRIVATE_SRCS}
        ${PRIVATE_HEADERS}
        ${PUBLIC_SRCS}
        ${PUBLIC_HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# CMakePackageConfig
################################################################################
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header blabby UPNPAV files" )

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/BlabbyMultimediaConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BlabbyUPnPAV
    PATH_VARS INCLUDE_INSTALL_DIR
)

write_basic_package_version_file(
    ${PROJECT_NAME}Version.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion
)

################################################################################
# Install
################################################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT BlabbyUPnPAV_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT BlabbyUPnPAV_Development
    NAMELINK_COMPONENT BlabbyUPnPAV_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${PUBLIC_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}")
install(FILES ${PRIVATE_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}/private")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
