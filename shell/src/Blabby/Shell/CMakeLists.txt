################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}
    URI "Blabby.Shell"
    VERSION 1.0
    STATIC
    QML_FILES
        MainWindow.qml
        RenderersView.qml
    RESOURCES
        ../../icons/24x24/folder.svg
        ../../icons/24x24/play_arrow.svg
        ../../icons/24x24/close.svg
        ../../icons/material/speaker.svg
)
add_library(BlabbyShell::Qml ALIAS ${PROJECT_NAME})
add_library(BlabbyShell::QmlPlugin ALIAS ${PROJECT_NAME}plugin)
