################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} SHARED)
add_library(Blabby::Artwork ALIAS ${PROJECT_NAME})
generate_export_header(${PROJECT_NAME})

set(PRIVATE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/Service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/MusicBrainzService.cpp
)

set(PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/Service.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/MusicBrainzService.hpp
)

set(PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lookup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Request.cpp
)

set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HttpBackend.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lookup.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Request.hpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PRIVATE_SRCS}
        ${PRIVATE_HEADERS}
        ${PUBLIC_SRCS}
        ${PUBLIC_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Network
        Qt6::Gui
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# The User-Agent of the lookup names the version of Blabby, not the one of this module.
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        BLABBY_VERSION="${CMAKE_PROJECT_VERSION}"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# CMakePackageConfig
################################################################################
set(ARTWORK_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header blabby artwork files" )

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/BlabbyArtworkConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BlabbyArtwork
    PATH_VARS ARTWORK_INCLUDE_INSTALL_DIR
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
    COMPONENT BlabbyArtwork_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT BlabbyArtwork_Development
    NAMELINK_COMPONENT BlabbyArtwork_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${PUBLIC_HEADERS} DESTINATION "${ARTWORK_INCLUDE_INSTALL_DIR}")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
