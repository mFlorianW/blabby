// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * A bar at the bottom of the screens that shows and controls what the Active Renderer plays.
 * A track area on the left shows the Current Track with its artwork, or a placeholder without, its title and its
 * artist, long texts are elided. Without an Active Renderer it reads "No Renderer", an Active Renderer without media
 * reads "Nothing playing". A tap on the track area asks to open the Playing screen in every state.
 * The previous, Play/Pause and next buttons in the middle follow the rules of the Playing screen: Play/Pause pauses,
 * or stops a Renderer that can't pause, and resumes or plays, it shows a busy ring while the Renderer is transitioning;
 * previous and next are disabled when unavailable and hidden while the Queue is empty. Without media all of them are
 * disabled.
 * A pill on the right shows the Active Renderer, or "Choose a Renderer" without one, and opens the @ref RendererMenu
 * upwards above it to pick the Active Renderer. The menu covers the item the mini player is laid out in, above its
 * other children, and closes when the mini player becomes invisible.
 */
Item {
    id: miniPlayer

    /**
     * True while there is an Active Renderer.
     */
    property bool hasActiveRenderer: false

    /**
     * The name of the Active Renderer.
     */
    property string rendererName

    /**
     * The Renderers to pick the Active Renderer from in the Renderer menu, a model with the roles "name",
     * "availability" and "active".
     */
    property alias renderers: rendererMenu.model

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
     * True when the Active Renderer can pause, otherwise the Play/Pause button stops it.
     */
    property bool canPause: true

    /**
     * True while the Active Renderer is transitioning, e.g. loading or buffering.
     */
    property bool transitioning: false

    /**
     * True while the Queue has entries.
     */
    property bool hasQueue: false

    /**
     * True when previous does something in the Queue.
     */
    property bool hasPrevious: false

    /**
     * True when next does something in the Queue.
     */
    property bool hasNext: false

    /**
     * True while the Active Renderer is Playing.
     */
    readonly property bool playing: miniPlayer.playbackState === Renderer.Playing

    /**
     * True while the Current Track is shown, i.e. the Active Renderer has media.
     */
    readonly property bool showsTrack: miniPlayer.hasActiveRenderer && miniPlayer.playbackState !== Renderer.NoMedia

    /**
     * This signal is emitted when the user asks to open the Playing screen.
     */
    signal nowPlayingRequested

    /**
     * This signal is emitted when the user asks to pause, stop, resume or play the Active Renderer.
     */
    signal togglePlaybackRequested

    /**
     * This signal is emitted when the user asks to restart the Current Entry or to play the preceding entry of the
     * Queue.
     */
    signal previousRequested

    /**
     * This signal is emitted when the user asks to play the next entry of the Queue.
     */
    signal nextRequested

    /**
     * This signal is emitted when the user picks the Renderer at index in the Renderer menu to make it the Active
     * Renderer.
     */
    signal rendererPicked(int index)

    implicitHeight: 80

    Rectangle {
        id: background
        objectName: "background"
        anchors.fill: miniPlayer
        radius: 24
        color: Theme.colors.surfaceContainerHigh
    }

    AbstractInteractiveControl {
        id: trackArea
        objectName: "trackArea"
        anchors.top: miniPlayer.top
        anchors.bottom: miniPlayer.bottom
        anchors.left: miniPlayer.left
        anchors.right: transportControls.left
        anchors.rightMargin: 16
        onClicked: miniPlayer.nowPlayingRequested()

        Artwork {
            id: artwork
            objectName: "trackArtwork"
            anchors.left: trackArea.left
            anchors.verticalCenter: trackArea.verticalCenter
            anchors.leftMargin: 12
            width: 56
            height: 56
            radius: 12
            source: miniPlayer.showsTrack ? miniPlayer.artworkUrl : ""
            glyphSource: "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"
        }

        Column {
            id: details
            anchors.left: artwork.right
            anchors.right: trackArea.right
            anchors.verticalCenter: trackArea.verticalCenter
            anchors.leftMargin: 16
            spacing: 2

            StyledText {
                id: title
                objectName: "trackTitle"
                width: details.width
                text: !miniPlayer.hasActiveRenderer ? qsTr("No Renderer") : miniPlayer.showsTrack ? miniPlayer.trackTitle : qsTr("Nothing playing")
                elide: Text.ElideRight
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.titleMedium
            }

            StyledText {
                id: artist
                objectName: "trackArtist"
                width: details.width
                text: miniPlayer.trackArtist
                elide: Text.ElideRight
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.bodyMedium
                visible: miniPlayer.showsTrack && miniPlayer.trackArtist !== ""
            }
        }
    }

    Row {
        id: transportControls
        anchors.centerIn: miniPlayer
        spacing: 8

        IconButton {
            id: previousButton
            objectName: "previousButton"
            anchors.verticalCenter: transportControls.verticalCenter
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/skip_previous.svg"
            visible: miniPlayer.hasQueue
            enabled: miniPlayer.showsTrack && miniPlayer.hasPrevious
            onClicked: miniPlayer.previousRequested()
        }

        AbstractInteractiveControl {
            id: playPauseButton
            objectName: "playPauseButton"
            width: 56
            height: 56
            enabled: miniPlayer.showsTrack
            opacity: playPauseButton.enabled ? 1 : Theme.disabledOpacity
            onClicked: miniPlayer.togglePlaybackRequested()

            Rectangle {
                id: playPauseContainer
                objectName: "container"
                anchors.fill: playPauseButton
                radius: playPauseButton.width / 2
                color: Theme.colors.primary
            }

            Rectangle {
                id: playPauseStateLayer
                anchors.fill: playPauseContainer
                radius: playPauseContainer.radius
                color: Theme.colors.colorOnPrimary
                opacity: playPauseButton.pressed ? Theme.stateLayer.pressedOpacity : playPauseButton.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            Icon {
                id: playPauseIcon
                objectName: "icon"
                anchors.centerIn: playPauseButton
                width: 28
                height: 28
                source: !miniPlayer.playing ? "qrc:/qt/qml/Blabby/Shell/icons/material/play_arrow.svg" : miniPlayer.canPause ? "qrc:/qt/qml/Blabby/Shell/icons/material/pause.svg" : "qrc:/qt/qml/Blabby/Shell/icons/material/stop.svg"
                color: Theme.colors.colorOnPrimary
            }

            BusyIndicator {
                id: busyRing
                objectName: "busyRing"
                anchors.centerIn: playPauseButton
                width: 44
                height: 44
                strokeWidth: 3
                color: Theme.colors.colorOnPrimary
                running: miniPlayer.transitioning
            }
        }

        IconButton {
            id: nextButton
            objectName: "nextButton"
            anchors.verticalCenter: transportControls.verticalCenter
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/skip_next.svg"
            visible: miniPlayer.hasQueue
            enabled: miniPlayer.showsTrack && miniPlayer.hasNext
            onClicked: miniPlayer.nextRequested()
        }
    }

    PillButton {
        id: rendererPill
        objectName: "rendererPill"
        anchors.right: miniPlayer.right
        anchors.verticalCenter: miniPlayer.verticalCenter
        anchors.rightMargin: 12
        text: miniPlayer.hasActiveRenderer ? miniPlayer.rendererName : qsTr("Choose a Renderer")
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
        checked: rendererMenu.opened
        // While the menu is open its tap catcher covers the pill, a tap on the pill closes the menu then.
        onClicked: rendererMenu.open()
    }

    RendererMenu {
        id: rendererMenu
        objectName: "rendererMenu"
        // The menu covers the item the mini player is laid out in, not only the bar, so a tap anywhere else closes it,
        // and is drawn above its other children, e.g. a toast.
        parent: miniPlayer.parent
        // Bound to parent, not miniPlayer.parent: the latter is evaluated before the reparenting and never again, so the
        // anchoring would target an item that isn't the parent of the menu.
        anchors.fill: parent
        z: 1
        // Being a child of the parent of the mini player, the menu doesn't become invisible with the mini player by
        // itself.
        visible: miniPlayer.visible
        anchorItem: rendererPill
        opensUpwards: true
        onPicked: index => miniPlayer.rendererPicked(index)
    }
}
