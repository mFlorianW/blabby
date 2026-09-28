// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * A tile in the Library grid that shows an Item with a square placeholder and its title below.
 * The placeholder carries a folder glyph for a Container and a note glyph for a Playable.
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

    implicitWidth: 200
    implicitHeight: placeholder.height + 8 + titleText.height

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
}
