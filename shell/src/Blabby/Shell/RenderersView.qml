// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls

/**
 * The screen to choose the Renderer that plays the music.
 * Every Online Renderer is shown as a card in a grid that reflows to the available width.
 */
Item {
    id: renderersView

    /**
     * The Renderers to show, a model with the roles "name", "playbackState" and "active".
     */
    property alias model: grid.model

    /**
     * This signal is emitted when the user taps the card of the Renderer at index.
     */
    signal activated(int index)

    /**
     * The smallest width of a card, used to calculate the number of columns.
     */
    readonly property real minimumCardWidth: 300

    /**
     * The height of a card, the implicit height of the RendererCard.
     */
    readonly property real cardHeight: 88

    /**
     * The space between two cards.
     */
    readonly property real cardSpacing: 16

    ScreenHeader {
        id: header
        anchors.top: renderersView.top
        anchors.left: renderersView.left
        anchors.right: renderersView.right
        anchors.margins: 24
        title: qsTr("Renderers")
        subtitle: qsTr("Choose where your music plays")
    }

    GridView {
        id: grid
        objectName: "rendererGrid"

        /**
         * The number of card columns that fit into the width of the grid.
         */
        readonly property int columns: Math.max(1, Math.floor(grid.width / (renderersView.minimumCardWidth + renderersView.cardSpacing)))

        anchors.top: header.bottom
        anchors.topMargin: 24
        anchors.bottom: renderersView.bottom
        anchors.left: header.left
        // The cells include the spacing on their right side, so the grid extends by one spacing.
        anchors.right: renderersView.right
        anchors.rightMargin: 24 - renderersView.cardSpacing
        clip: true
        cellWidth: grid.width / grid.columns
        cellHeight: renderersView.cardHeight + renderersView.cardSpacing
        boundsBehavior: Flickable.StopAtBounds

        delegate: Item {
            id: cell
            required property int index
            required property string name
            required property int playbackState
            required property bool active

            width: grid.cellWidth
            height: grid.cellHeight

            RendererCard {
                objectName: "rendererCard" + cell.index
                anchors.left: cell.left
                anchors.right: cell.right
                anchors.rightMargin: renderersView.cardSpacing
                anchors.top: cell.top
                height: renderersView.cardHeight
                name: cell.name
                playbackState: cell.playbackState
                selected: cell.active
                onClicked: renderersView.activated(cell.index)
            }
        }
    }
}
