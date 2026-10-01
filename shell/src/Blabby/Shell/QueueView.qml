// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme
import "TimeFormat.js" as TimeFormat

/**
 * The Queue screen that lists the entries of the Queue.
 * The header tells the number of tracks and their total duration, prefixed with "≥" when some durations are unknown
 * and left out when none is known. Each row shows the number, the artwork, the title, the artist, the album and the
 * duration of its entry, the parts that are unknown are left out. The Current Entry is highlighted with an equaliser
 * glyph instead of its number, which moves while the Active Renderer plays it for the Running Queue. Tapping a row asks to play its entry,
 * without an Active Renderer a toast tells to choose one and leads to the Renderers screen.
 * Swiping a row away asks to remove its entry, dragging a row by its handle asks to move it, Clear in the header asks to
 * remove all entries. Remove and Clear ask no confirmation, their toast offers to undo them instead.
 * An empty Queue shows an empty state that leads to the Library.
 */
Item {
    id: queueView

    /**
     * The entries of the Queue, a model with the roles "title", "artist", "album", "artworkUrl", "duration",
     * "hasDuration" and "current".
     */
    property alias entries: entryList.model

    /**
     * True while there is an Active Renderer.
     */
    property bool hasActiveRenderer: false

    /**
     * True while the Active Renderer plays the Current Entry for the Queue.
     */
    property bool currentEntryPlaying: false

    /**
     * The number of entries.
     */
    property int entryCount: 0

    /**
     * The sum of the known durations of the entries in milliseconds.
     */
    property real totalDuration: 0

    /**
     * True when the duration of at least one entry is known.
     */
    property bool hasTotalDuration: false

    /**
     * True when the duration of at least one entry is unknown.
     */
    property bool totalDurationPartial: false

    /**
     * The summary in the header, e.g. "8 tracks · 34 min".
     */
    readonly property string summary: {
        const tracks = queueView.entryCount === 1 ? qsTr("1 track") : qsTr("%1 tracks").arg(queueView.entryCount);
        if (!queueView.hasTotalDuration) {
            return tracks;
        }
        if (queueView.totalDurationPartial) {
            // A lower bound is rounded down to never promise more than is known, less than a minute tells nothing.
            const knownMinutes = Math.floor(queueView.totalDuration / 60000);
            return knownMinutes > 0 ? `${tracks} · ${qsTr("≥ %1 min").arg(knownMinutes)}` : tracks;
        }
        // A Queue that plays at all lasts at least a minute in the summary.
        const minutes = Math.max(1, Math.round(queueView.totalDuration / 60000));
        return `${tracks} · ${qsTr("%1 min").arg(minutes)}`;
    }

    /**
     * True when the Queue has no entries.
     */
    readonly property bool empty: entryList.count === 0

    /**
     * Shows the message in the toast with an action to undo what it tells.
     */
    function offerUndo(message: string) {
        toast.undoable = true;
        toast.show(message, qsTr("Undo"));
    }

    /**
     * This signal is emitted when the user taps the row of the entry at index to play it.
     */
    signal playRequested(int index)

    /**
     * This signal is emitted when the user swiped the row of the entry at index away to remove it.
     */
    signal removeRequested(int index)

    /**
     * This signal is emitted when the user dragged the row of the entry at from by its handle to the row at to.
     */
    signal moveRequested(int from, int to)

    /**
     * This signal is emitted when the user asks to remove all entries.
     */
    signal clearRequested

    /**
     * This signal is emitted when the user asks to undo the last remove or clear.
     */
    signal undoRequested

    /**
     * This signal is emitted when the user asks to choose the Active Renderer.
     */
    signal chooseRendererRequested

    /**
     * This signal is emitted when the user asks to browse the Library to fill the Queue.
     */
    signal browseLibraryRequested

    ScreenHeader {
        id: header
        objectName: "header"
        anchors.top: queueView.top
        anchors.left: queueView.left
        anchors.right: queueView.right
        anchors.margins: 24
        title: qsTr("Queue")
        subtitle: queueView.summary

        Button {
            id: clearButton
            objectName: "clearButton"
            text: qsTr("Clear")
            visible: !queueView.empty
            onClicked: {
                queueView.clearRequested();
                queueView.offerUndo(qsTr("Cleared the Queue"));
            }
        }
    }

    EmptyState {
        id: emptyState
        objectName: "emptyState"
        anchors.top: header.bottom
        anchors.bottom: queueView.bottom
        anchors.left: queueView.left
        anchors.right: queueView.right
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/queue_music.svg"
        title: qsTr("The Queue is empty")
        hint: qsTr("Tap a track in the Library to add it to the Queue and play it")
        actionText: qsTr("Browse the Library")
        visible: queueView.empty
        onActionClicked: queueView.browseLibraryRequested()
    }

    ListView {
        id: entryList
        objectName: "entryList"
        anchors.top: header.bottom
        anchors.topMargin: 16
        anchors.bottom: queueView.bottom
        anchors.left: queueView.left
        anchors.right: queueView.right
        anchors.leftMargin: 24
        anchors.rightMargin: 40
        clip: true
        spacing: 2
        boundsBehavior: Flickable.StopAtBounds
        visible: !queueView.empty

        delegate: AbstractInteractiveControl {
            id: entryRow
            required property int index
            required property string title
            required property string artist
            required property string album
            required property string artworkUrl
            required property real duration
            required property bool hasDuration
            required property bool current

            // A row swiped this far away is removed when it's released, otherwise it slides back.
            readonly property real removeDistance: entryRow.width / 3

            // The height of a row including the spacing, a row dragged this far moves by one row.
            readonly property real rowPitch: entryRow.height + entryList.spacing

            // How far the row is swiped from its place.
            property real swipeDistance: 0

            objectName: "entryRow" + entryRow.index
            width: entryList.width
            height: 60
            // The dragged row is drawn above the rows it passes.
            z: dragArea.pressed ? 1 : 0
            transform: [
                Translate {
                    id: swipeOffset
                    objectName: "swipeOffset"
                    x: entryRow.swipeDistance

                    Behavior on x {
                        enabled: !swipeHandler.active
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.OutQuad
                        }
                    }
                },
                Translate {
                    id: dragOffset
                    y: dragArea.pressed ? dragArea.offset : 0
                }
            ]
            opacity: 1 - Math.min(Math.abs(swipeOffset.x) / entryRow.width, 0.6)

            onClicked: {
                queueView.playRequested(entryRow.index);
                if (!queueView.hasActiveRenderer) {
                    toast.undoable = false;
                    toast.show(qsTr("Choose a Renderer to play the Queue"), qsTr("Choose Renderer"));
                }
            }

            DragHandler {
                id: swipeHandler
                objectName: "swipeHandler"
                target: null
                yAxis.enabled: false
                onTranslationChanged: delta => entryRow.swipeDistance += delta.x
                onActiveChanged: {
                    if (swipeHandler.active) {
                        return;
                    }
                    const removed = Math.abs(entryRow.swipeDistance) >= entryRow.removeDistance;
                    entryRow.swipeDistance = 0;
                    if (removed) {
                        // The row is gone once its entry is removed.
                        queueView.offerUndo(qsTr("Removed “%1”").arg(entryRow.title));
                        queueView.removeRequested(entryRow.index);
                    }
                }
            }

            Rectangle {
                id: background
                objectName: "background"
                anchors.fill: entryRow
                radius: 16
                color: entryRow.current ? Theme.colors.surfaceContainerHigh : "transparent"
            }

            Rectangle {
                id: stateLayer
                anchors.fill: entryRow
                radius: background.radius
                color: Theme.colors.colorOnSurface
                opacity: entryRow.pressed ? Theme.stateLayer.pressedOpacity : entryRow.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            Item {
                id: numberSlot
                anchors.left: entryRow.left
                anchors.leftMargin: 12
                anchors.verticalCenter: entryRow.verticalCenter
                width: 32
                height: 32

                StyledText {
                    id: number
                    objectName: "number"
                    anchors.centerIn: numberSlot
                    text: entryRow.index + 1
                    color: Theme.colors.colorOnSurfaceVariant
                    textStyle: Theme.fonts.labelLarge
                    visible: !entryRow.current
                }

                EqualizerGlyph {
                    id: equalizer
                    objectName: "equalizer"
                    anchors.centerIn: numberSlot
                    width: 22
                    height: 22
                    color: Theme.colors.primary
                    running: queueView.currentEntryPlaying
                    visible: entryRow.current
                }
            }

            Artwork {
                id: artwork
                objectName: "artwork"
                anchors.left: numberSlot.right
                anchors.leftMargin: 16
                anchors.verticalCenter: entryRow.verticalCenter
                width: 44
                height: 44
                radius: 8
                source: entryRow.artworkUrl
                glyphSource: "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"
            }

            Column {
                id: titleAndArtist
                anchors.left: artwork.right
                anchors.right: albumText.left
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                anchors.verticalCenter: entryRow.verticalCenter

                StyledText {
                    id: titleText
                    objectName: "title"
                    width: titleAndArtist.width
                    text: entryRow.title
                    elide: Text.ElideRight
                    color: entryRow.current ? Theme.colors.primary : Theme.colors.colorOnSurface
                    textStyle: Theme.fonts.titleMedium
                }

                StyledText {
                    id: artistText
                    objectName: "artist"
                    width: titleAndArtist.width
                    text: entryRow.artist
                    elide: Text.ElideRight
                    color: Theme.colors.colorOnSurfaceVariant
                    textStyle: Theme.fonts.bodyMedium
                    visible: entryRow.artist !== ""
                }
            }

            // The album keeps its column when it is left out, so the columns of the rows stay aligned.
            StyledText {
                id: albumText
                objectName: "album"
                anchors.right: durationText.left
                anchors.rightMargin: 16
                anchors.verticalCenter: entryRow.verticalCenter
                width: Math.min(240, entryRow.width / 4)
                text: entryRow.album
                elide: Text.ElideRight
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.bodyMedium
                visible: entryRow.album !== ""
            }

            StyledText {
                id: durationText
                objectName: "duration"
                anchors.right: dragHandle.left
                anchors.rightMargin: 8
                anchors.verticalCenter: entryRow.verticalCenter
                width: 48
                horizontalAlignment: Text.AlignRight
                text: TimeFormat.formatTime(entryRow.duration)
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.labelMedium
                visible: entryRow.hasDuration
            }

            Item {
                id: dragHandle
                objectName: "dragHandle"
                anchors.right: entryRow.right
                anchors.rightMargin: 8
                anchors.verticalCenter: entryRow.verticalCenter
                width: 48
                height: 48

                Icon {
                    id: dragHandleIcon
                    anchors.centerIn: dragHandle
                    width: 24
                    height: 24
                    source: "qrc:/qt/qml/Blabby/Shell/icons/material/drag_handle.svg"
                    color: Theme.colors.colorOnSurfaceVariant
                }

                MouseArea {
                    id: dragArea

                    // How far the row is dragged from its place.
                    property real offset: 0
                    property real pressY: 0

                    anchors.fill: dragHandle
                    // The list must not scroll instead of moving the row.
                    preventStealing: true
                    // The pointer is mapped to the list, the handle moves along with the row.
                    function offsetOf(mouse: var): real {
                        return dragArea.mapToItem(entryList.contentItem, mouse.x, mouse.y).y - dragArea.pressY;
                    }

                    onPressed: mouse => {
                        dragArea.pressY = dragArea.mapToItem(entryList.contentItem, mouse.x, mouse.y).y;
                        dragArea.offset = 0;
                    }
                    onPositionChanged: mouse => dragArea.offset = dragArea.offsetOf(mouse)
                    onReleased: mouse => {
                        dragArea.offset = dragArea.offsetOf(mouse);
                        const rows = Math.round(dragArea.offset / entryRow.rowPitch);
                        const to = Math.max(0, Math.min(entryList.count - 1, entryRow.index + rows));
                        dragArea.offset = 0;
                        if (to !== entryRow.index) {
                            queueView.moveRequested(entryRow.index, to);
                        }
                    }
                }
            }
        }
    }

    Toast {
        id: toast
        objectName: "toast"
        anchors.horizontalCenter: queueView.horizontalCenter
        anchors.bottom: queueView.bottom
        anchors.bottomMargin: 24
        maximumWidth: queueView.width - 48
        onActionClicked: toast.undoable ? queueView.undoRequested() : queueView.chooseRendererRequested()

        // True when the action of the shown message undoes a remove or a clear, it chooses a Renderer otherwise.
        property bool undoable: false
    }
}
