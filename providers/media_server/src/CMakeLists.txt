################################################################################
# Build
################################################################################
add_library(${PROJECT_NAME} SHARED)
add_library(Blabby::MediaServerProvider ALIAS ${PROJECT_NAME})
generate_export_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaServerSource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/private/LoggingCategories.hpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Quick
        Blabby::Multimedia
        Blabby::UPnPAV
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

################################################################################
# Resources
################################################################################
qt_add_resources(${PROJECT_NAME} "mediaserverprovider"
    PREFIX "/mediaserverprovider"
    FILES
        icons/24x24/PC.svg
)

################################################################################
# Install
################################################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/blabby/plugins
    COMPONENT BlabbyUPnPAV_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/blabby/plugins
    COMPONENT BlabbyUPnPAV_Development
    NAMELINK_COMPONENT BlabbyUPnPAV_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/blabby/plugins
)
