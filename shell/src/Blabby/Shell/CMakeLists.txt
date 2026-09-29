################################################################################
# QML Module
################################################################################
qt_add_qml_module(${PROJECT_NAME}
    URI "Blabby.Shell"
    VERSION 1.0
    STATIC
    QML_FILES
        LibraryTile.qml
        LibraryView.qml
        MainWindow.qml
        NowPlayingView.qml
        RendererCard.qml
        RenderersView.qml
        SourcePicker.qml
    RESOURCES
        ../../icons/24x24/play_arrow.svg
        ../../icons/24x24/close.svg
        ../../icons/material/arrow_back.svg
        ../../icons/material/delete.svg
        ../../icons/material/dns.svg
        ../../icons/material/expand_more.svg
        ../../icons/material/folder.svg
        ../../icons/material/graphic_eq.svg
        ../../icons/material/library_music.svg
        ../../icons/material/music_note.svg
        ../../icons/material/pause.svg
        ../../icons/material/play_arrow.svg
        ../../icons/material/play_circle.svg
        ../../icons/material/refresh.svg
        ../../icons/material/speaker.svg
        ../../icons/material/stop.svg
        ../../icons/material/volume_off.svg
        ../../icons/material/volume_up.svg
)
add_library(BlabbyShell::Qml ALIAS ${PROJECT_NAME})
add_library(BlabbyShell::QmlPlugin ALIAS ${PROJECT_NAME}plugin)
