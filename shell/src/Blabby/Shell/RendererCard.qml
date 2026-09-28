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
 */
Card {
    id: rendererCard

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
     * How the Playback State is shown: its text, the optional icon and the emphasis.
     * No Media shows nothing.
     */
    readonly property var playbackStatePresentation: {
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

    implicitHeight: 100

    IconBadge {
        id: badge
        anchors.left: rendererCard.contentItem.left
        anchors.verticalCenter: rendererCard.contentItem.verticalCenter
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
        emphasis: rendererCard.selected
    }

    Column {
        id: textColumn
        anchors.left: badge.right
        anchors.leftMargin: 16
        anchors.right: rendererCard.contentItem.right
        anchors.verticalCenter: rendererCard.contentItem.verticalCenter
        spacing: 2

        StyledText {
            id: nameText
            objectName: "name"
            width: textColumn.width
            text: rendererCard.name
            color: rendererCard.contentColor
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
