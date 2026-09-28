// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls

/**
 * The screen to choose the Renderer that plays the music.
 * Every known Renderer is shown as a card in a grid that reflows to the available width, Offline ones can't be tapped.
 * Without Renderers an empty state tells whether the network is still searched or nothing was found.
 */
Item {
    id: renderersView

    /**
     * The Renderers to show, a model with the roles "name", "manufacturer", "modelName", "address",
     * "playbackState", "active" and "availability".
     */
    property alias model: grid.model

    /**
     * True while the network is scanned for Renderers.
     */
    property bool scanning: false

    /**
     * This signal is emitted when the user taps the card of the Renderer at index.
     */
    signal activated(int index)

    /**
     * This signal is emitted when the user asks for a new scan of the network.
     */
    signal rescanRequested

    /**
     * The smallest width of a card, used to calculate the number of columns.
     */
    readonly property real minimumCardWidth: 300

    /**
     * The height of a card, the implicit height of the RendererCard.
     */
    readonly property real cardHeight: 100

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

        Button {
            id: rescanButton
            objectName: "rescanButton"
            variant: Button.Outlined
            text: qsTr("Rescan network")
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/refresh.svg"
            busy: renderersView.scanning
            onClicked: renderersView.rescanRequested()
        }
    }

    EmptyState {
        id: emptyState
        objectName: "emptyState"
        anchors.top: header.bottom
        anchors.bottom: renderersView.bottom
        anchors.left: renderersView.left
        anchors.right: renderersView.right
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
        busy: renderersView.scanning
        title: renderersView.scanning ? qsTr("Searching…") : qsTr("No renderers found")
        hint: renderersView.scanning ? "" : qsTr("Make sure your speakers are switched on and on the same network")
        visible: grid.count === 0
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
            required property string manufacturer
            required property string modelName
            required property string address
            required property int playbackState
            required property bool active
            required property int availability

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
                manufacturer: cell.manufacturer
                modelName: cell.modelName
                address: cell.address
                playbackState: cell.playbackState
                selected: cell.active
                availability: cell.availability
                onClicked: renderersView.activated(cell.index)
            }
        }
    }
}
