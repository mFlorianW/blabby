################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}Controls STATIC
    URI
        "Blabby.Controls"
    VERSION
        1.0
    QML_FILES
        Artwork.qml
        BusyIndicator.qml
        Button.qml
        Card.qml
        EmptyState.qml
        EqualizerGlyph.qml
        IconButton.qml
        IconBadge.qml
        Icon.qml
        Dialog.qml
        FadeBehavior.qml
        NavigationRail.qml
        NavigationRailItem.qml
        PillButton.qml
        ScreenHeader.qml
        Slider.qml
        StatusLabel.qml
        StyledText.qml
        Toast.qml
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractInteractiveControl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractInteractiveControl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.hpp
)
add_library(BlabbyShell::Controls ALIAS ${PROJECT_NAME}Controls)
add_library(BlabbyShell::ControlsPlugin ALIAS ${PROJECT_NAME}Controlsplugin)

target_link_libraries(${PROJECT_NAME}Controls
    PUBLIC
        Qt6::Quick
)
