// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * The Playing screen that shows what the Active Renderer plays.
 * A Renderer pill in the header shows the Active Renderer and asks to choose another one.
 * Without an Active Renderer an empty state asks to choose one, an Active Renderer without media tells that nothing
 * plays on it. Otherwise the Current Track is shown with its artwork, or a placeholder without, its title and its
 * artist when known. When the Active Renderer went Offline a toast tells so.
 */
Item {
    id: nowPlayingView

    /**
     * True while there is an Active Renderer.
     */
    property bool hasActiveRenderer: false

    /**
     * The name of the Active Renderer.
     */
    property string rendererName

    /**
     * The Playback State of the Active Renderer, a value of Renderer.State.
     */
    property int playbackState: Renderer.NoMedia

    /**
     * The title of the Current Track.
     */
    property string trackTitle

    /**
     * The artist of the Current Track, empty when unknown.
     */
    property string trackArtist

    /**
     * The URL of the artwork of the Current Track, empty when unknown.
     */
    property url artworkUrl

    /**
     * True while the Current Track is shown, i.e. the Active Renderer has media.
     */
    readonly property bool showsTrack: nowPlayingView.hasActiveRenderer && nowPlayingView.playbackState !== Renderer.NoMedia

    /**
     * This signal is emitted when the user asks to choose the Active Renderer.
     */
    signal chooseRendererRequested

    /**
     * Tells the user in a toast that the Active Renderer with the name went Offline.
     */
    function showActiveRendererWentOffline(rendererName: string) {
        toast.show(qsTr("%1 is no longer available").arg(rendererName));
    }

    ScreenHeader {
        id: header
        anchors.top: nowPlayingView.top
        anchors.left: nowPlayingView.left
        anchors.right: nowPlayingView.right
        anchors.margins: 24
        title: qsTr("Now Playing")
        subtitle: qsTr("Control what your Renderer plays")

        PillButton {
            id: rendererPill
            objectName: "rendererPill"
            text: nowPlayingView.rendererName
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            visible: nowPlayingView.hasActiveRenderer
            onClicked: nowPlayingView.chooseRendererRequested()
        }
    }

    EmptyState {
        id: emptyState
        objectName: "emptyState"
        anchors.top: header.bottom
        anchors.bottom: nowPlayingView.bottom
        anchors.left: nowPlayingView.left
        anchors.right: nowPlayingView.right
        iconSource: nowPlayingView.hasActiveRenderer ? "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg" : "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
        title: nowPlayingView.hasActiveRenderer ? qsTr("Nothing playing on %1").arg(nowPlayingView.rendererName) : qsTr("Choose a Renderer")
        hint: nowPlayingView.hasActiveRenderer ? "" : qsTr("Pick the Renderer whose playback you want to control")
        actionText: nowPlayingView.hasActiveRenderer ? "" : qsTr("Choose Renderer")
        visible: !nowPlayingView.showsTrack
        onActionClicked: nowPlayingView.chooseRendererRequested()
    }

    Item {
        id: track
        objectName: "track"
        anchors.top: header.bottom
        anchors.bottom: nowPlayingView.bottom
        anchors.left: nowPlayingView.left
        anchors.right: nowPlayingView.right
        anchors.topMargin: 24
        anchors.bottomMargin: 32
        anchors.leftMargin: 24
        anchors.rightMargin: 40
        visible: nowPlayingView.showsTrack

        Artwork {
            id: artwork
            objectName: "trackArtwork"
            anchors.left: track.left
            anchors.verticalCenter: track.verticalCenter
            width: Math.min(track.height, track.width * 0.42)
            height: artwork.width
            radius: 28
            source: nowPlayingView.artworkUrl
            glyphSource: "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"
        }

        Column {
            id: details
            anchors.left: artwork.right
            anchors.right: track.right
            anchors.verticalCenter: track.verticalCenter
            anchors.leftMargin: 48
            spacing: 8

            StyledText {
                id: title
                objectName: "trackTitle"
                width: details.width
                text: nowPlayingView.trackTitle
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.displayMedium
            }

            StyledText {
                id: artist
                objectName: "trackArtist"
                width: details.width
                text: nowPlayingView.trackArtist
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.titleLarge
                visible: nowPlayingView.trackArtist !== ""
            }
        }
    }

    Toast {
        id: toast
        objectName: "toast"
        anchors.horizontalCenter: nowPlayingView.horizontalCenter
        anchors.bottom: nowPlayingView.bottom
        anchors.bottomMargin: 24
        maximumWidth: nowPlayingView.width - 48
    }
}
