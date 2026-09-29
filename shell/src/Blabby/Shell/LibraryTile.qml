// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * A tile in the Library grid that shows an Item with its square artwork, its title and its secondary text below.
 * Without artwork, or when the artwork can't be loaded, a placeholder carries a folder glyph for a Container and
 * a note glyph for a Playable. The secondary text, e.g. the artist, is only shown when there is one.
 */
AbstractInteractiveControl {
    id: tile

    /**
     * The title of the Item.
     */
    property string title

    /**
     * The type of the Item, a value of ItemType.
     */
    property int itemType: ItemType.Container

    /**
     * The secondary text of the Item, e.g. the artist, empty when it has none.
     */
    property string secondaryText

    /**
     * The URL of the artwork of the Item, e.g. the album art, empty when it has none.
     */
    property url artworkUrl

    /**
     * True when the artwork is loaded and shown instead of the placeholder glyph.
     */
    readonly property bool hasArtwork: artworkImage.status === Image.Ready

    implicitWidth: 200
    implicitHeight: placeholder.height + 8 + titleText.height + (secondaryTextLabel.visible ? secondaryTextLabel.height : 0)

    Rectangle {
        id: placeholder
        objectName: "placeholder"
        anchors.top: tile.top
        anchors.left: tile.left
        anchors.right: tile.right
        height: placeholder.width
        radius: 16
        color: Theme.colors.secondaryContainer

        Icon {
            id: glyph
            objectName: "glyph"
            anchors.centerIn: placeholder
            width: placeholder.width * 0.4
            height: placeholder.height * 0.4
            source: tile.itemType === ItemType.Container ? "qrc:/qt/qml/Blabby/Shell/icons/material/folder.svg" : "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"
            color: Theme.colors.colorOnSecondaryContainer
            opacity: 0.5
            visible: !tile.hasArtwork
        }

        Image {
            id: artworkImage
            anchors.fill: placeholder
            source: tile.artworkUrl
            sourceSize.width: placeholder.width
            sourceSize.height: placeholder.height
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            visible: false
        }

        Rectangle {
            id: artworkMask
            anchors.fill: placeholder
            radius: placeholder.radius
            visible: false
            layer.enabled: true
        }

        // Rounds the corners of the artwork like the placeholder.
        MultiEffect {
            id: artwork
            objectName: "artwork"
            anchors.fill: placeholder
            source: artworkImage
            maskEnabled: true
            maskSource: artworkMask
            visible: tile.hasArtwork
        }

        Rectangle {
            id: stateLayer
            anchors.fill: placeholder
            radius: placeholder.radius
            color: Theme.colors.colorOnSurface
            opacity: tile.pressed ? Theme.stateLayer.pressedOpacity : tile.hovered ? Theme.stateLayer.hoverOpacity : 0
        }
    }

    StyledText {
        id: titleText
        objectName: "title"
        anchors.top: placeholder.bottom
        anchors.topMargin: 8
        anchors.left: tile.left
        anchors.leftMargin: 4
        anchors.right: tile.right
        anchors.rightMargin: 4
        text: tile.title
        color: Theme.colors.colorOnSurface
        textStyle: Theme.fonts.titleMedium
    }

    StyledText {
        id: secondaryTextLabel
        objectName: "secondaryText"
        anchors.top: titleText.bottom
        anchors.left: titleText.left
        anchors.right: titleText.right
        text: tile.secondaryText
        color: Theme.colors.colorOnSurfaceVariant
        textStyle: Theme.fonts.bodyMedium
        visible: tile.secondaryText !== ""
    }
}
