################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}Objects
    URI
        "Blabby.Objects"
    VERSION
        1.0
    STATIC
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/MultimediaTypes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MultimediaTypes.hpp
)
add_library(BlabbyShell::Objects ALIAS ${PROJECT_NAME}Objects)
add_library(BlabbyShell::ObjectsPlugin ALIAS ${PROJECT_NAME}Objectsplugin)

target_link_libraries(${PROJECT_NAME}Objects
    PUBLIC
        Qt6::Qml
        Blabby::Multimedia
)
