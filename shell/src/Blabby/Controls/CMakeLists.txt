################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}Controls STATIC
    URI
        "Blabby.Controls"
    VERSION
        1.0
    QML_FILES
        Clock.qml
        IconButton.qml
        ListEntry.qml
        Header.qml
        Dialog.qml
        FadeBehavior.qml
        Switch.qml
        Slider.qml
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractClock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractIconButton.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractIconButton.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractListEntry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractListEntry.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractHeader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractDialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractDialog.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractInteractiveControl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractInteractiveControl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSwitch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSwitch.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LoggingCategories.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSlider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AbstractSlider.hpp
)
add_library(BlabbyShell::Controls ALIAS ${PROJECT_NAME}Controls)
add_library(BlabbyShell::ControlsPlugin ALIAS ${PROJECT_NAME}Controlsplugin)

target_link_libraries(${PROJECT_NAME}Controls
    PUBLIC
        Qt6::Quick
)
