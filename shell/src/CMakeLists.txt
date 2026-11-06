################################################################################
# Configure File
################################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ProviderDirs.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/ProviderDirs.hpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BlabbyVersion.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/BlabbyVersion.hpp)

################################################################################
# QML Modules
################################################################################
add_subdirectory(Blabby/Shell)
add_subdirectory(Blabby/Objects)
add_subdirectory(Blabby/Controls)
add_subdirectory(Blabby/Theme)

################################################################################
# Build blabby
################################################################################
add_library(${PROJECT_NAME}Object OBJECT)
add_library(BlabbyShell::Object ALIAS ${PROJECT_NAME}Object)

target_sources(${PROJECT_NAME}Object
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ActiveRendererController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ActiveRendererController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ArtworkImageProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ArtworkImageProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ArtworkUrl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ArtworkUrl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FolderArtwork.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FolderArtwork.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IniSettingsStore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IniSettingsStore.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoadProviders.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoadProviders.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaSourceModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QmlSingletonRegistry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QmlSingletonRegistry.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaItemModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaRendererModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QueueModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QueueModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsRowModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsRowModel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsStore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SettingsStore.hpp
)

target_link_libraries(${PROJECT_NAME}Object
    PUBLIC
        Blabby::UPnPAV
        Blabby::Multimedia
        Blabby::Artwork
        BlabbyShell::Qml
        BlabbyShell::QmlPlugin
        BlabbyShell::Objects
        BlabbyShell::ObjectsPlugin
        BlabbyShell::Controls
        BlabbyShell::ControlsPlugin
        BlabbyShell::Theme
        BlabbyShell::ThemePlugin
)

target_include_directories(${PROJECT_NAME}Object
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/qml>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_executable(blabby)
target_sources(blabby
    PRIVATE
        main.cpp
)

target_link_libraries(blabby
    PRIVATE
        BlabbyShell::Object
)

qt_add_resources(blabby "blabby_icons"
    PREFIX "/blabby"
    FILES
        icons/blabby.svg
)

################################################################################
# Install
################################################################################
blabby_set_install_rpath(blabby)
install(TARGETS blabby
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT BlabbyUPnPAV_Runtime)

