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
 * plays on it. Otherwise the Current Track is shown with its artwork, or a placeholder without, its title, and its
 * artist, "album · year" and a format chip with the parts that are known. When the Active Renderer went Offline a
 * toast tells so. A large Play/Pause button pauses, or stops a Renderer that can't pause, and resumes or plays; it
 * shows a busy ring while the Renderer is transitioning. A seek bar shows the elapsed and the total time and seeks on
 * release or a tap, it only shows the position when the Renderer can't seek and only the elapsed time is shown for a
 * stream without a duration. Failed control calls are told in a toast.
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
     * The album of the Current Track, empty when unknown.
     */
    property string trackAlbum

    /**
     * The year of the Current Track, empty when unknown.
     */
    property string trackYear

    /**
     * The format of the Current Track, e.g. "FLAC · 24-bit / 96 kHz", empty when unknown.
     */
    property string trackFormat

    /**
     * True when the Active Renderer can pause, otherwise the Play/Pause button stops it.
     */
    property bool canPause: true

    /**
     * True while the Active Renderer is transitioning, e.g. loading or buffering.
     */
    property bool transitioning: false

    /**
     * The position in the Current Track in milliseconds.
     */
    property real position: 0

    /**
     * True when the duration of the Current Track is known, it's unknown e.g. for a stream.
     */
    property bool hasDuration: false

    /**
     * The duration of the Current Track in milliseconds.
     */
    property real duration: 0

    /**
     * True when the Active Renderer can seek in the Current Track.
     */
    property bool canSeek: false

    /**
     * The position a seek was requested to, -1 without. The seek bar stays there until the next position update.
     */
    property real seekTarget: -1

    /**
     * True while the Active Renderer is Playing.
     */
    readonly property bool playing: nowPlayingView.playbackState === Renderer.Playing

    /**
     * True while the Active Renderer is Stopped.
     */
    readonly property bool stopped: nowPlayingView.playbackState === Renderer.Stopped

    /**
     * True while the Current Track is shown, i.e. the Active Renderer has media.
     */
    readonly property bool showsTrack: nowPlayingView.hasActiveRenderer && nowPlayingView.playbackState !== Renderer.NoMedia

    /**
     * This signal is emitted when the user asks to choose the Active Renderer.
     */
    signal chooseRendererRequested

    /**
     * This signal is emitted when the user asks to pause, stop, resume or play the Active Renderer.
     */
    signal togglePlaybackRequested

    /**
     * This signal is emitted when the user asks to seek to the position in milliseconds.
     */
    signal seekRequested(real position)

    /**
     * Shows the position again instead of the target of a requested seek, e.g. because the seek failed.
     */
    function cancelSeek() {
        nowPlayingView.seekTarget = -1;
    }

    /**
     * Gives the time in milliseconds as m:ss, or h:mm:ss from an hour on.
     */
    function formatTime(milliseconds: real): string {
        const totalSeconds = Math.floor(milliseconds / 1000);
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor(totalSeconds / 60) % 60;
        const seconds = String(totalSeconds % 60).padStart(2, "0");
        if (hours > 0) {
            return `${hours}:${String(minutes).padStart(2, "0")}:${seconds}`;
        }
        return `${minutes}:${seconds}`;
    }

    onPositionChanged: nowPlayingView.cancelSeek()

    /**
     * Tells the user in a toast that a control call to the Renderer with the name failed.
     * @param action The action of the failed call, a value of Renderer.Action.
     */
    function showControlFailed(rendererName: string, action: int) {
        const messages = {
            [Renderer.Play]: qsTr("Couldn't play %1"),
            [Renderer.Resume]: qsTr("Couldn't resume %1"),
            [Renderer.Pause]: qsTr("Couldn't pause %1"),
            [Renderer.Stop]: qsTr("Couldn't stop %1"),
            [Renderer.Seek]: qsTr("Couldn't seek on %1")
        };
        if (action === Renderer.Seek) {
            nowPlayingView.cancelSeek();
        }
        toast.show(messages[action].arg(rendererName));
    }

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

            Rectangle {
                id: formatChip
                objectName: "formatChip"
                width: formatRow.implicitWidth + 24
                height: 32
                radius: 8
                color: "transparent"
                border.color: Theme.colors.outlineVariant
                border.width: 1
                visible: nowPlayingView.trackFormat !== ""

                Row {
                    id: formatRow
                    anchors.centerIn: formatChip
                    spacing: 6

                    Icon {
                        anchors.verticalCenter: formatRow.verticalCenter
                        width: 18
                        height: 18
                        source: "qrc:/qt/qml/Blabby/Shell/icons/material/graphic_eq.svg"
                        color: Theme.colors.primary
                    }

                    StyledText {
                        objectName: "formatText"
                        anchors.verticalCenter: formatRow.verticalCenter
                        text: nowPlayingView.trackFormat
                        color: Theme.colors.colorOnSurfaceVariant
                        textStyle: Theme.fonts.labelLarge
                    }
                }
            }

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

            StyledText {
                id: albumAndYear
                objectName: "trackAlbumAndYear"
                width: details.width
                text: [nowPlayingView.trackAlbum, nowPlayingView.trackYear].filter(part => part !== "").join(" · ")
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.bodyLarge
                visible: albumAndYear.text !== ""
            }

            Item {
                id: seekBarSpacing
                width: details.width
                height: 20
            }

            Column {
                id: seekBar
                width: details.width
                spacing: 4

                Slider {
                    id: seekSlider
                    objectName: "seekSlider"
                    width: seekBar.width
                    from: 0
                    to: nowPlayingView.duration
                    value: nowPlayingView.seekTarget >= 0 ? nowPlayingView.seekTarget : nowPlayingView.stopped ? 0 : nowPlayingView.position
                    interactive: nowPlayingView.canSeek && !nowPlayingView.stopped
                    valueIndicatorEnabled: true
                    valueIndicatorText: nowPlayingView.formatTime(seekSlider.pressedValue)
                    visible: nowPlayingView.hasDuration
                    onCommitted: value => {
                        nowPlayingView.seekTarget = value;
                        nowPlayingView.seekRequested(value);
                    }
                }

                Item {
                    id: times
                    width: seekBar.width
                    height: elapsedTime.height

                    StyledText {
                        id: elapsedTime
                        objectName: "elapsedTime"
                        anchors.left: times.left
                        // The elapsed time keeps the playing position while seeking.
                        text: nowPlayingView.formatTime(nowPlayingView.stopped ? 0 : nowPlayingView.position)
                        color: Theme.colors.colorOnSurfaceVariant
                        textStyle: Theme.fonts.labelLarge
                    }

                    StyledText {
                        id: totalTime
                        objectName: "totalTime"
                        anchors.right: times.right
                        text: nowPlayingView.formatTime(nowPlayingView.duration)
                        color: Theme.colors.colorOnSurfaceVariant
                        textStyle: Theme.fonts.labelLarge
                        visible: nowPlayingView.hasDuration
                    }
                }
            }

            Item {
                id: controlsSpacing
                width: details.width
                height: 20
            }

            AbstractInteractiveControl {
                id: playPauseButton
                objectName: "playPauseButton"
                width: 128
                height: 96
                onClicked: nowPlayingView.togglePlaybackRequested()

                Rectangle {
                    id: playPauseContainer
                    objectName: "container"
                    anchors.fill: playPauseButton
                    // The button is rounder while not playing, as in the design.
                    radius: nowPlayingView.playing ? 28 : 48
                    color: Theme.colors.primary

                    Behavior on radius {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutQuad
                        }
                    }
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
                    width: 44
                    height: 44
                    source: !nowPlayingView.playing ? "qrc:/qt/qml/Blabby/Shell/icons/material/play_arrow.svg" : nowPlayingView.canPause ? "qrc:/qt/qml/Blabby/Shell/icons/material/pause.svg" : "qrc:/qt/qml/Blabby/Shell/icons/material/stop.svg"
                    color: Theme.colors.colorOnPrimary
                }

                BusyIndicator {
                    id: busyRing
                    objectName: "busyRing"
                    anchors.centerIn: playPauseButton
                    width: 72
                    height: 72
                    strokeWidth: 4
                    color: Theme.colors.colorOnPrimary
                    running: nowPlayingView.transitioning
                }
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
