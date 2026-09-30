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
     * This signal is emitted when the user taps the row of the entry at index to play it.
     */
    signal playRequested(int index)

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

            objectName: "entryRow" + entryRow.index
            width: entryList.width
            height: 60
            onClicked: {
                queueView.playRequested(entryRow.index);
                if (!queueView.hasActiveRenderer) {
                    toast.show(qsTr("Choose a Renderer to play the Queue"), qsTr("Choose Renderer"));
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
                anchors.right: entryRow.right
                anchors.rightMargin: 16
                anchors.verticalCenter: entryRow.verticalCenter
                width: 48
                horizontalAlignment: Text.AlignRight
                text: TimeFormat.formatTime(entryRow.duration)
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.labelMedium
                visible: entryRow.hasDuration
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
        onActionClicked: queueView.chooseRendererRequested()
    }
}
