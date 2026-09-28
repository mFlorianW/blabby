// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * The screen to browse the media of the Active Source.
 * The Items of the current Container are shown as tiles in a grid that reflows to the available width.
 * A Source pill in the header shows the Active Source and opens the Source picker.
 * Without an Active Source an empty state asks to choose one, or tells that no Source was found.
 */
Item {
    id: libraryView

    /**
     * The Sources to pick from, a model with the roles "mediaSourceName" and "mediaSourceIconUrl".
     */
    property alias sources: sourcePicker.model

    /**
     * The Items of the current Container, a model with the roles "mediaItemTitle" and "mediaItemType".
     */
    property alias items: grid.model

    /**
     * True while there is an Active Source.
     */
    property bool hasActiveSource: false

    /**
     * The name of the Active Source.
     */
    property string activeSourceName

    /**
     * True when there is at least one Source to pick.
     */
    readonly property bool hasSources: sourcePicker.count > 0

    /**
     * This signal is emitted when the user picks the Source at index in the Source picker.
     */
    signal sourcePicked(int index)

    /**
     * This signal is emitted when the user taps the tile of the Item at index.
     */
    signal itemActivated(int index)

    /**
     * The smallest width of a tile, used to calculate the number of columns.
     */
    readonly property real minimumTileWidth: 200

    /**
     * The horizontal space between two tiles.
     */
    readonly property real tileSpacing: 16

    /**
     * The vertical space between two rows of tiles.
     */
    readonly property real rowSpacing: 24

    ScreenHeader {
        id: header
        anchors.top: libraryView.top
        anchors.left: libraryView.left
        anchors.right: libraryView.right
        anchors.margins: 24
        title: qsTr("Library")
        subtitle: qsTr("Browse the media on your network")

        PillButton {
            id: sourcePill
            objectName: "sourcePill"
            text: libraryView.activeSourceName
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
            visible: libraryView.hasActiveSource
            onClicked: sourcePicker.visible = true
        }
    }

    EmptyState {
        id: emptyState
        objectName: "emptyState"
        anchors.top: header.bottom
        anchors.bottom: libraryView.bottom
        anchors.left: libraryView.left
        anchors.right: libraryView.right
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/library_music.svg"
        title: libraryView.hasSources ? qsTr("Choose a Source") : qsTr("No Sources found")
        hint: libraryView.hasSources ? qsTr("Pick the Source whose media you want to browse") : qsTr("Make sure your media servers are switched on and on the same network")
        actionText: libraryView.hasSources ? qsTr("Choose Source") : ""
        visible: !libraryView.hasActiveSource
        onActionClicked: sourcePicker.visible = true
    }

    GridView {
        id: grid
        objectName: "itemGrid"

        /**
         * The number of tile columns that fit into the width of the grid.
         */
        readonly property int columns: Math.max(1, Math.floor(grid.width / (libraryView.minimumTileWidth + libraryView.tileSpacing)))

        /**
         * The width of a tile, the cell width without the spacing.
         */
        readonly property real tileWidth: grid.cellWidth - libraryView.tileSpacing

        anchors.top: header.bottom
        anchors.topMargin: 24
        anchors.bottom: libraryView.bottom
        anchors.left: header.left
        // The cells include the spacing on their right side, so the grid extends by one spacing.
        anchors.right: libraryView.right
        anchors.rightMargin: 24 - libraryView.tileSpacing
        clip: true
        cellWidth: grid.width / grid.columns
        cellHeight: grid.tileWidth + 8 + Theme.fonts.titleMedium.lineHeight + libraryView.rowSpacing
        boundsBehavior: Flickable.StopAtBounds
        visible: libraryView.hasActiveSource

        delegate: Item {
            id: cell
            required property int index
            required property string mediaItemTitle
            required property int mediaItemType

            width: grid.cellWidth
            height: grid.cellHeight

            LibraryTile {
                objectName: "itemTile" + cell.index
                anchors.left: cell.left
                anchors.top: cell.top
                width: grid.tileWidth
                title: cell.mediaItemTitle
                itemType: cell.mediaItemType
                onClicked: libraryView.itemActivated(cell.index)
            }
        }
    }

    Rectangle {
        id: scrim
        objectName: "scrim"
        anchors.fill: libraryView
        color: Theme.colors.surfaceContainerLowest
        opacity: 0.6
        visible: sourcePicker.visible

        // Closes the picker when the user taps beside it and keeps the taps from the content below.
        MouseArea {
            anchors.fill: scrim
            onClicked: sourcePicker.visible = false
        }
    }

    SourcePicker {
        id: sourcePicker
        objectName: "sourcePicker"
        anchors.centerIn: libraryView
        visible: false
        onPicked: index => {
            sourcePicker.visible = false;
            libraryView.sourcePicked(index);
        }
    }
}
