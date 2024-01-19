add_library(${PROJECT_NAME} SHARED)
add_library(Blabby::Multimedia ALIAS ${PROJECT_NAME})
generate_export_header(${PROJECT_NAME})

set(PRIVATE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/NavigationStack.cpp
)

set(PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/NavigationStack.hpp
)

set(PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Provider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Item.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RendererProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
)

set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Provider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProviderLoader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Item.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RendererProvider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.hpp
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
        Blabby::UPnPAV
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# CMakePackageConfig
################################################################################
set(MULTIMEDIA_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header blabby multimedia files" )

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
    COMPONENT BlabbyMultimedia_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT BlabbyMultimedia_Development
    NAMELINK_COMPONENT BlabbyUPnPAV_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${PUBLIC_HEADERS} DESTINATION "${MULTIMEDIA_INCLUDE_DIR}")
install(FILES ${PRIVATE_HEADERS} DESTINATION "${MULTIMEDIA_INCLUDE_DIR}/private")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
