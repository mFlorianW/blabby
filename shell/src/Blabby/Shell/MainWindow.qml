// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import Blabby.Shell
import Blabby.Controls
import Blabby.Singleton
import Blabby.Theme

Rectangle {
    id: shell
    color: Theme.colors.surface

    /**
     * The indexes of the destinations in the navigation rail.
     */
    readonly property int playingDestination: 0
    readonly property int libraryDestination: 1
    readonly property int renderersDestination: 2

    /**
     * True while the Renderers screen was opened from the Playing screen, picking a Renderer returns to it then.
     */
    property bool returnToPlaying: false

    NavigationRail {
        id: rail
        anchors.top: shell.top
        anchors.bottom: shell.bottom
        anchors.left: shell.left
        currentIndex: shell.libraryDestination
        model: [
            {
                "text": qsTr("Playing"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/play_circle.svg"
            },
            {
                "text": qsTr("Library"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/library_music.svg"
            },
            {
                "text": qsTr("Renderers"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            }
        ]
        onActivated: shell.returnToPlaying = false
    }

    StackLayout {
        id: destinations
        anchors.top: shell.top
        anchors.bottom: shell.bottom
        anchors.left: rail.right
        anchors.right: shell.right
        currentIndex: rail.currentIndex

        NowPlayingView {
            id: nowPlayingView
            hasActiveRenderer: Singleton.activeRendererController.hasActiveRenderer
            rendererName: Singleton.activeRendererController.rendererName
            playbackState: Singleton.activeRendererController.playbackState
            trackTitle: Singleton.activeRendererController.trackTitle
            trackArtist: Singleton.activeRendererController.trackArtist
            artworkUrl: Singleton.activeRendererController.artworkUrl
            trackAlbum: Singleton.activeRendererController.trackAlbum
            trackYear: Singleton.activeRendererController.trackYear
            trackFormat: Singleton.activeRendererController.trackFormat
            canPause: Singleton.activeRendererController.canPause
            transitioning: Singleton.activeRendererController.transitioning
            position: Singleton.activeRendererController.position
            hasDuration: Singleton.activeRendererController.hasDuration
            duration: Singleton.activeRendererController.duration
            canSeek: Singleton.activeRendererController.canSeek
            volume: Singleton.activeRendererController.volume
            volumeMinimum: Singleton.activeRendererController.volumeMinimum
            volumeMaximum: Singleton.activeRendererController.volumeMaximum
            canControlVolume: Singleton.activeRendererController.canControlVolume
            onChooseRendererRequested: {
                shell.returnToPlaying = true;
                rail.currentIndex = shell.renderersDestination;
            }

            onTogglePlaybackRequested: Singleton.activeRendererController.togglePlayback()
            onSeekRequested: position => Singleton.activeRendererController.seek(position)
            onVolumeRequested: volume => Singleton.activeRendererController.setVolume(volume)

            Connections {
                target: Singleton.activeRendererController
                function onActiveRendererWentOffline(rendererName: string) {
                    nowPlayingView.showActiveRendererWentOffline(rendererName);
                }
                function onControlFailed(rendererName: string, action: int) {
                    nowPlayingView.showControlFailed(rendererName, action);
                }
            }
        }

        LibraryView {
            id: libraryView
            sources: Singleton.mediaSourceModel
            items: Singleton.mediaItemModel
            hasActiveSource: Singleton.mediaItemModel.hasMediaSource
            activeSourceName: Singleton.mediaItemModel.mediaSourceName
            busy: Singleton.mediaItemModel.busy
            atRoot: Singleton.mediaItemModel.atRoot
            containerTitle: Singleton.mediaItemModel.containerTitle
            onSourcePicked: index => Singleton.mediaSourceModel.activateMediaSource(index)
            onItemActivated: index => Singleton.mediaItemModel.activateMediaItem(index)
            onBackRequested: Singleton.mediaItemModel.navigateBack()

            Connections {
                target: Singleton.mediaItemModel
                function onContainerOpenFailed(containerTitle: string) {
                    libraryView.showContainerOpenFailed(containerTitle);
                }
            }
        }

        RenderersView {
            id: renderersView
            model: Singleton.mediaRendererModel
            scanning: Singleton.mediaRendererModel.scanning
            onRescanRequested: Singleton.mediaRendererModel.rescan()
            onActivated: index => {
                Singleton.mediaRendererModel.activateRenderer(Singleton.mediaRendererModel.index(index, 0));
                if (shell.returnToPlaying) {
                    shell.returnToPlaying = false;
                    rail.currentIndex = shell.playingDestination;
                }
            }
            onForgetRequested: index => Singleton.mediaRendererModel.forgetRenderer(Singleton.mediaRendererModel.index(index, 0))
        }
    }
}
