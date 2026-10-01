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
 * A Source pill in the header shows the Active Source, or "Choose a Source" without one, and opens and closes the
 * Source menu below it.
 * Below the root Container a back button next to the Source pill leads to the parent Container and the title of the
 * current Container is shown above the grid. While a Container opens a busy indicator replaces the grid, when it can't
 * be opened the current Container stays and a toast tells so.
 * When loading more Items of a large Container fails, a row at the end of the grid offers to retry it.
 * When the Active Source disappears a toast tells so.
 * Without an Active Source an empty state asks to choose one, or tells that no Source was found.
 */
Item {
    id: libraryView

    /**
     * The Sources to pick from, a model with the roles "mediaSourceName" and "mediaSourceIconUrl".
     */
    property alias sources: sourceMenu.model

    /**
     * The Items of the current Container, a model with the roles "mediaItemTitle", "mediaItemType",
     * "mediaItemSecondaryText" and "mediaItemArtworkUrl".
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
     * True while a Container opens, a busy indicator replaces the grid meanwhile.
     */
    property bool busy: false

    /**
     * True while the current Container is the root Container of the Active Source.
     */
    property bool atRoot: true

    /**
     * The title of the current Container, shown above the grid below the root Container.
     */
    property string containerTitle

    /**
     * True when loading more Items of the current Container failed, a row at the end of the grid offers to retry it.
     */
    property bool loadMoreFailed: false

    /**
     * True when there is at least one Source to pick.
     */
    readonly property bool hasSources: sourceMenu.count > 0

    /**
     * True while the Items of a Container below the root Container are browsed.
     */
    readonly property bool belowRoot: libraryView.hasActiveSource && !libraryView.atRoot

    /**
     * True when the current Container has been opened and holds no Items.
     */
    readonly property bool containerEmpty: libraryView.hasActiveSource && !libraryView.busy && grid.count === 0

    /**
     * The edge below which the grid and the empty state start, below the Container title when it is shown.
     */
    readonly property var contentTop: containerTitleText.visible ? containerTitleText.bottom : header.bottom

    /**
     * This signal is emitted when the user picks the Source at index in the Source menu.
     */
    signal sourcePicked(int index)

    /**
     * This signal is emitted when the user taps the tile of the Item at index, to open a Container or to play a
     * Playable.
     */
    signal itemActivated(int index)

    /**
     * This signal is emitted when the user taps the back button to return to the parent Container.
     */
    signal backRequested

    /**
     * This signal is emitted when the user taps the Retry button to load more Items again after it failed.
     */
    signal retryRequested

    /**
     * Tells the user in a toast that the Container with the title couldn't be opened.
     */
    function showContainerOpenFailed(containerTitle: string) {
        toast.show(qsTr("Couldn't open %1").arg(containerTitle));
    }

    /**
     * Tells the user in a toast that the Active Source with the name is no longer available.
     */
    function showActiveSourceDisappeared(sourceName: string) {
        toast.show(qsTr("%1 is no longer available").arg(sourceName));
    }

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

        IconButton {
            id: backButton
            objectName: "backButton"
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/arrow_back.svg"
            anchors.verticalCenter: sourcePill.verticalCenter
            visible: libraryView.belowRoot
            onClicked: libraryView.backRequested()
        }

        PillButton {
            id: sourcePill
            objectName: "sourcePill"
            text: libraryView.hasActiveSource ? libraryView.activeSourceName : qsTr("Choose a Source")
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
            showsChevron: true
            checked: sourceMenu.opened
            // While the menu is open its tap catcher covers the pill, a tap on the pill closes the menu then.
            onClicked: sourceMenu.open()
        }
    }

    StyledText {
        id: containerTitleText
        objectName: "containerTitle"
        anchors.top: header.bottom
        anchors.topMargin: 24
        anchors.left: header.left
        anchors.right: header.right
        text: libraryView.containerTitle
        color: Theme.colors.colorOnSurface
        textStyle: Theme.fonts.titleLarge
        visible: libraryView.belowRoot
    }

    EmptyState {
        id: emptyState
        objectName: "emptyState"
        anchors.top: libraryView.contentTop
        anchors.bottom: libraryView.bottom
        anchors.left: libraryView.left
        anchors.right: libraryView.right
        iconSource: libraryView.hasActiveSource ? "qrc:/qt/qml/Blabby/Shell/icons/material/folder.svg" : "qrc:/qt/qml/Blabby/Shell/icons/material/library_music.svg"
        title: libraryView.hasActiveSource ? qsTr("Nothing in here") : libraryView.hasSources ? qsTr("Choose a Source") : qsTr("No Sources found")
        hint: libraryView.hasActiveSource ? "" : libraryView.hasSources ? qsTr("Pick the Source whose media you want to browse") : qsTr("Make sure your media servers are switched on and on the same network")
        actionText: !libraryView.hasActiveSource && libraryView.hasSources ? qsTr("Choose Source") : ""
        visible: !libraryView.hasActiveSource || libraryView.containerEmpty
        onActionClicked: sourceMenu.open()
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

        anchors.top: libraryView.contentTop
        anchors.topMargin: 24
        anchors.bottom: libraryView.bottom
        anchors.left: header.left
        // The cells include the spacing on their right side, so the grid extends by one spacing.
        anchors.right: libraryView.right
        anchors.rightMargin: 24 - libraryView.tileSpacing
        clip: true
        cellWidth: grid.width / grid.columns
        // Every cell leaves room for the secondary text, so the rows stay aligned.
        cellHeight: grid.tileWidth + 8 + Theme.fonts.titleMedium.lineHeight + Theme.fonts.bodyMedium.lineHeight + libraryView.rowSpacing
        boundsBehavior: Flickable.StopAtBounds
        visible: libraryView.hasActiveSource && !libraryView.busy && !libraryView.containerEmpty

        delegate: Item {
            id: cell
            required property int index
            required property string mediaItemTitle
            required property int mediaItemType
            required property string mediaItemSecondaryText
            required property string mediaItemArtworkUrl

            width: grid.cellWidth
            height: grid.cellHeight

            LibraryTile {
                objectName: "itemTile" + cell.index
                anchors.left: cell.left
                anchors.top: cell.top
                width: grid.tileWidth
                title: cell.mediaItemTitle
                itemType: cell.mediaItemType
                secondaryText: cell.mediaItemSecondaryText
                artworkUrl: cell.mediaItemArtworkUrl
                onClicked: libraryView.itemActivated(cell.index)
            }
        }

        footer: Item {
            objectName: "loadMoreFailedRow"
            // As wide as the tiles of a row, without the spacing behind the last tile.
            width: grid.width - libraryView.tileSpacing
            height: visible ? retryButton.height : 0
            visible: libraryView.loadMoreFailed

            Row {
                anchors.centerIn: parent
                spacing: 8

                StyledText {
                    objectName: "message"
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Couldn't load more")
                    color: Theme.colors.colorOnSurfaceVariant
                    textStyle: Theme.fonts.bodyMedium
                }

                Button {
                    id: retryButton
                    objectName: "retryButton"
                    // Taller than the default button to be a large enough touch target.
                    height: 48
                    variant: Button.Text
                    text: qsTr("Retry")
                    onClicked: libraryView.retryRequested()
                }
            }
        }
    }

    BusyIndicator {
        id: containerBusyIndicator
        objectName: "containerBusyIndicator"
        anchors.centerIn: grid
        width: 48
        height: 48
        strokeWidth: 4
        color: Theme.colors.primary
        running: libraryView.hasActiveSource && libraryView.busy
    }

    Toast {
        id: toast
        objectName: "toast"
        anchors.horizontalCenter: libraryView.horizontalCenter
        anchors.bottom: libraryView.bottom
        anchors.bottomMargin: 24
        maximumWidth: libraryView.width - 48
    }

    SourceMenu {
        id: sourceMenu
        objectName: "sourceMenu"
        anchors.fill: libraryView
        anchorItem: sourcePill
        onPicked: index => libraryView.sourcePicked(index)
    }
}
