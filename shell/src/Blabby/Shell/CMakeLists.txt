
################################################################################
# QML Module
################################################################################
set_source_files_properties(Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(${PROJECT_NAME}
    URI "Blabby.Shell"
    VERSION 1.0
    STATIC
    QML_FILES
        MainWindow.qml
        Theme.qml
        Menu.qml
    RESOURCES
        ../../icons/24x24/burgermenu.svg
        ../../icons/24x24/navigate_back.svg
        ../../icons/24x24/navigate_next.svg
        ../../icons/24x24/folder.svg
        ../../icons/24x24/play_arrow.svg
        ../../icons/24x24/close.svg
)
add_library(BlabbyShell::Qml ALIAS ${PROJECT_NAME})
add_library(BlabbyShell::QmlPlugin ALIAS ${PROJECT_NAME}plugin)
