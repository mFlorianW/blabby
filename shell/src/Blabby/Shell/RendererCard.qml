// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * A card that shows a Renderer with its name, details (manufacturer, model and address) and Playback State.
 * The card is highlighted while the Renderer is the Active Renderer.
 * An Offline Renderer shows Offline instead of the Playback State with its last known details and can't be tapped,
 * instead it offers to forget the Renderer with a trailing button that isn't dimmed.
 */
Item {
    id: rendererCard

    /**
     * True when the Renderer is the Active Renderer.
     */
    property alias selected: card.selected

    /**
     * The name of the Renderer.
     */
    property string name

    /**
     * The manufacturer of the Renderer, empty when unknown.
     */
    property string manufacturer

    /**
     * The model name of the Renderer, empty when unknown.
     */
    property string modelName

    /**
     * The network address of the Renderer, empty when unknown.
     */
    property string address

    /**
     * The details that tell similarly named Renderers apart: manufacturer, model and address
     * separated by dots, missing or blank fields are omitted.
     */
    readonly property string details: [rendererCard.manufacturer, rendererCard.modelName, rendererCard.address].map(field => field.trim()).filter(field => field !== "").join(" · ")

    /**
     * The Playback State of the Renderer, a value of Renderer.State.
     */
    property int playbackState: Renderer.NoMedia

    /**
     * The Availability of the Renderer, a value of Renderer.Availability.
     */
    property int availability: Renderer.Online

    /**
     * How the Playback State is shown: its text, the optional icon and the emphasis.
     * No Media shows nothing, an Offline Renderer shows Offline in place of its Playback State.
     */
    readonly property var playbackStatePresentation: {
        if (rendererCard.availability === Renderer.Offline) {
            return {
                "text": qsTr("Offline"),
                "iconSource": "",
                "emphasis": false
            };
        }

        switch (rendererCard.playbackState) {
        case Renderer.Playing:
            return {
                "text": qsTr("Playing"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg",
                "emphasis": true
            };
        case Renderer.Paused:
            return {
                "text": qsTr("Paused"),
                "iconSource": "",
                "emphasis": false
            };
        case Renderer.Stopped:
            return {
                "text": qsTr("Stopped"),
                "iconSource": "",
                "emphasis": false
            };
        default:
            return {
                "text": "",
                "iconSource": "",
                "emphasis": false
            };
        }
    }

    /**
     * True when the Renderer can be forgotten, only Offline Renderers can.
     */
    readonly property bool forgettable: rendererCard.availability === Renderer.Offline

    /**
     * This signal is emitted when the user taps the card of an Online Renderer.
     */
    signal clicked

    /**
     * This signal is emitted when the user asks to forget the Offline Renderer.
     */
    signal forgetRequested

    implicitHeight: 100

    Card {
        id: card
        objectName: "card"
        anchors.fill: rendererCard
        enabled: rendererCard.availability === Renderer.Online
        onClicked: rendererCard.clicked()

        IconBadge {
            id: badge
            anchors.left: card.contentItem.left
            anchors.verticalCenter: card.contentItem.verticalCenter
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            emphasis: card.selected
        }

        Column {
            id: textColumn
            anchors.left: badge.right
            anchors.leftMargin: 16
            anchors.right: card.contentItem.right
            // Leaves room for the forget button, which is placed on top of the card.
            anchors.rightMargin: rendererCard.forgettable ? forgetButton.width : 0
            anchors.verticalCenter: card.contentItem.verticalCenter
            spacing: 2

            StyledText {
                id: nameText
                objectName: "name"
                width: textColumn.width
                text: rendererCard.name
                color: card.contentColor
                textStyle: Theme.fonts.titleMedium
            }

            StyledText {
                id: detailsText
                objectName: "details"
                width: textColumn.width
                text: rendererCard.details
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.bodyMedium
                visible: rendererCard.details !== ""
            }

            StatusLabel {
                id: playbackStateLabel
                objectName: "playbackState"
                text: rendererCard.playbackStatePresentation.text
                iconSource: rendererCard.playbackStatePresentation.iconSource
                emphasis: rendererCard.playbackStatePresentation.emphasis
                visible: rendererCard.playbackStatePresentation.text !== ""
            }
        }
    }

    // A sibling of the card, so that it's neither dimmed nor disabled with the card of an Offline Renderer.
    IconButton {
        id: forgetButton
        objectName: "forgetButton"
        anchors.right: rendererCard.right
        anchors.rightMargin: 8
        anchors.verticalCenter: rendererCard.verticalCenter
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/delete.svg"
        visible: rendererCard.forgettable
        onClicked: rendererCard.forgetRequested()
    }
}
