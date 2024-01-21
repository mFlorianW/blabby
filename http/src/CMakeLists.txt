################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} SHARED)
add_library(Blabby::Http ALIAS ${PROJECT_NAME})
generate_export_header(${PROJECT_NAME})

set(PRIVATE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ClientConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/RequestReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseSerializer.cpp
)

set(PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ServerPrivate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ClientConnection.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/RequestReader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/ResponseSerializer.hpp
)

set(PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerRequest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerResponse.cpp
)

set(PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerRequest.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerResponse.hpp
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
    PRIVATE
        ${LLHTTP_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

################################################################################
# CMakePackageConfig
################################################################################
set(HTTP_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    CACHE PATH "Location of header blabby http files" )

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/BlabbyHttpConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BlabbyHttp
    PATH_VARS HTTP_INCLUDE_INSTALL_DIR
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
install(FILES ${PUBLIC_HEADERS} DESTINATION "${HTTP_INCLUDE_INSTALL_DIR}")
install(FILES ${PRIVATE_HEADERS} DESTINATION "${HTTP_INCLUDE_INSTALL_DIR}/private")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
