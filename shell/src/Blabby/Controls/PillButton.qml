// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A tonal pill in the header that shows a selection, e.g. the Active Source, with a leading icon,
 * its text and a trailing drop down arrow. Clicking it lets the user change the selection.
 */
AbstractInteractiveControl {
    id: pillButton

    /**
     * The text of the selection.
     */
    property string text

    /**
     * The URL of the leading SVG icon.
     */
    property url iconSource

    /**
     * The colour of the icons and the text.
     */
    readonly property color contentColor: Theme.colors.colorOnSecondaryContainer

    implicitHeight: 56
    implicitWidth: content.implicitWidth + 16 + 16

    Rectangle {
        id: background
        objectName: "background"
        anchors.fill: pillButton
        radius: pillButton.height / 2
        color: Theme.colors.secondaryContainer
    }

    Rectangle {
        id: stateLayer
        anchors.fill: background
        radius: background.radius
        color: pillButton.contentColor
        opacity: pillButton.pressed ? Theme.stateLayer.pressedOpacity : pillButton.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    Row {
        id: content
        anchors.centerIn: pillButton
        spacing: 8

        Icon {
            id: icon
            objectName: "icon"
            anchors.verticalCenter: content.verticalCenter
            width: 20
            height: 20
            source: pillButton.iconSource
            color: pillButton.contentColor
        }

        StyledText {
            id: label
            objectName: "label"
            anchors.verticalCenter: content.verticalCenter
            text: pillButton.text
            color: pillButton.contentColor
            textStyle: Theme.fonts.labelLarge
        }

        Icon {
            id: dropDownIcon
            objectName: "dropDownIcon"
            anchors.verticalCenter: content.verticalCenter
            width: 20
            height: 20
            source: "qrc:/qt/qml/Blabby/Shell/icons/material/expand_more.svg"
            color: pillButton.contentColor
        }
    }
}
