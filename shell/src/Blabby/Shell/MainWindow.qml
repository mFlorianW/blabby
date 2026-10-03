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
    readonly property int queueDestination: 2
    readonly property int renderersDestination: 3

    /**
     * The destination the Renderers screen was opened from to choose a Renderer, picking a Renderer returns to it.
     * -1 when the Renderers screen was opened from the navigation rail.
     */
    property int returnDestination: -1

    /**
     * Opens the Renderers screen to choose a Renderer, picking one returns to the destination.
     */
    function chooseRenderer(destination: int) {
        shell.returnDestination = destination;
        rail.currentIndex = shell.renderersDestination;
    }

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
                "text": qsTr("Queue"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/queue_music.svg"
            },
            {
                "text": qsTr("Renderers"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            }
        ]
        onActivated: shell.returnDestination = -1
    }

    StackLayout {
        id: destinations
        anchors.top: shell.top
        anchors.bottom: miniPlayer.visible ? miniPlayer.top : shell.bottom
        anchors.bottomMargin: miniPlayer.visible ? 8 : 0
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
            muted: Singleton.activeRendererController.muted
            canControlMute: Singleton.activeRendererController.canControlMute
            hasQueue: Singleton.queueModel.entryCount > 0
            hasPrevious: Singleton.queueModel.hasPrevious
            hasNext: Singleton.queueModel.hasNext
            onChooseRendererRequested: shell.chooseRenderer(shell.playingDestination)
            onQueueRequested: rail.currentIndex = shell.queueDestination

            onTogglePlaybackRequested: Singleton.queueModel.togglePlayback()
            onPreviousRequested: Singleton.queueModel.previous()
            onNextRequested: Singleton.queueModel.next()
            onSeekRequested: position => Singleton.activeRendererController.seek(position)
            onVolumeRequested: volume => Singleton.activeRendererController.setVolume(volume)
            onMuteRequested: muted => Singleton.activeRendererController.setMuted(muted)
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
            loadMoreFailed: Singleton.mediaItemModel.loadMoreFailed
            collecting: Singleton.queueModel.collecting
            collectedContainerTitle: Singleton.queueModel.collectedContainerTitle
            onSourcePicked: index => Singleton.mediaSourceModel.activateMediaSource(index)
            onItemActivated: (index, scrollPosition) => Singleton.mediaItemModel.activateMediaItem(index, scrollPosition)
            onBackRequested: Singleton.mediaItemModel.navigateBack()
            onRetryRequested: Singleton.mediaItemModel.retryLoadMore()
            onPlayNextRequested: index => Singleton.mediaItemModel.playMediaItemNext(index)
            onAddToQueueRequested: index => Singleton.mediaItemModel.addMediaItemToQueue(index)
            onCancelCollectionRequested: Singleton.queueModel.cancelCollection()

            Connections {
                target: Singleton.mediaItemModel
                function onContainerOpenFailed(containerTitle: string) {
                    libraryView.showContainerOpenFailed(containerTitle);
                }
                function onScrollPositionRestoreRequested(scrollPosition: int) {
                    libraryView.restoreScrollPosition(scrollPosition);
                }
            }

            Connections {
                target: Singleton.mediaSourceModel
                function onActiveMediaSourceDisappeared(sourceName: string) {
                    libraryView.showActiveSourceDisappeared(sourceName);
                }
            }

            Connections {
                target: Singleton.queueModel
                function onCollectionFailed(containerTitle: string) {
                    libraryView.showCollectionFailed(containerTitle);
                }
            }
        }

        QueueView {
            id: queueView
            entries: Singleton.queueModel
            hasActiveRenderer: Singleton.activeRendererController.hasActiveRenderer
            currentEntryPlaying: Singleton.queueModel.currentEntryPlaying
            entryCount: Singleton.queueModel.entryCount
            totalDuration: Singleton.queueModel.totalDuration
            hasTotalDuration: Singleton.queueModel.hasTotalDuration
            totalDurationPartial: Singleton.queueModel.totalDurationPartial
            onPlayRequested: index => Singleton.queueModel.play(index)
            onRemoveRequested: index => Singleton.queueModel.remove(index)
            onMoveRequested: (from, to) => Singleton.queueModel.move(from, to)
            onClearRequested: Singleton.queueModel.clear()
            onUndoRequested: Singleton.queueModel.undo()
            onChooseRendererRequested: shell.chooseRenderer(shell.queueDestination)
            onBrowseLibraryRequested: rail.currentIndex = shell.libraryDestination
        }

        RenderersView {
            id: renderersView
            model: Singleton.mediaRendererModel
            scanning: Singleton.mediaRendererModel.scanning
            onRescanRequested: Singleton.mediaRendererModel.rescan()
            onActivated: index => {
                Singleton.mediaRendererModel.activateRenderer(Singleton.mediaRendererModel.index(index, 0));
                if (shell.returnDestination >= 0) {
                    rail.currentIndex = shell.returnDestination;
                    shell.returnDestination = -1;
                }
            }
            onForgetRequested: index => Singleton.mediaRendererModel.forgetRenderer(Singleton.mediaRendererModel.index(index, 0))
        }
    }

    /**
     * Shows and controls what plays on every screen but the Playing screen, below the screen content.
     */
    MiniPlayer {
        id: miniPlayer
        anchors.bottom: shell.bottom
        anchors.left: rail.right
        anchors.right: shell.right
        anchors.margins: 16
        visible: rail.currentIndex !== shell.playingDestination
        hasActiveRenderer: Singleton.activeRendererController.hasActiveRenderer
        rendererName: Singleton.activeRendererController.rendererName
        playbackState: Singleton.activeRendererController.playbackState
        trackTitle: Singleton.activeRendererController.trackTitle
        trackArtist: Singleton.activeRendererController.trackArtist
        artworkUrl: Singleton.activeRendererController.artworkUrl
        canPause: Singleton.activeRendererController.canPause
        transitioning: Singleton.activeRendererController.transitioning
        hasQueue: Singleton.queueModel.entryCount > 0
        hasPrevious: Singleton.queueModel.hasPrevious
        hasNext: Singleton.queueModel.hasNext
        onNowPlayingRequested: rail.currentIndex = shell.playingDestination
        onTogglePlaybackRequested: Singleton.queueModel.togglePlayback()
        onPreviousRequested: Singleton.queueModel.previous()
        onNextRequested: Singleton.queueModel.next()
    }

    ControlFailureMessage {
        id: controlFailureMessage
    }

    Connections {
        target: Singleton.activeRendererController
        function onActiveRendererWentOffline(rendererName: string) {
            toast.show(qsTr("%1 is no longer available").arg(rendererName));
        }
        function onControlFailed(rendererName: string, action: int) {
            nowPlayingView.handleControlFailed(action);
            toast.show(controlFailureMessage.message(rendererName, action));
        }
    }

    /**
     * Tells about failed control calls and the Active Renderer going Offline on every screen, above the mini player.
     */
    Toast {
        id: toast
        objectName: "toast"
        anchors.horizontalCenter: destinations.horizontalCenter
        anchors.bottom: destinations.bottom
        anchors.bottomMargin: 24
        maximumWidth: destinations.width - 48
    }
}
