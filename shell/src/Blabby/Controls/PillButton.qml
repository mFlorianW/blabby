// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A tonal pill in the header that shows a selection, e.g. the Active Source, with a leading icon and its text.
 * Clicking it lets the user change the selection. An opt-in trailing chevron hints that the pill opens a menu, it turns
 * by 180° while the pill is checked, e.g. while its menu is open. A checked pill is drawn in the highlighted container
 * colours.
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
     * True to show the trailing chevron, false by default.
     */
    property bool showsChevron: false

    /**
     * True while the pill is checked, e.g. while its menu is open, false by default.
     */
    property bool checked: false

    /**
     * The colour of the icons and the text.
     */
    readonly property color contentColor: pillButton.checked ? Theme.colors.colorOnPrimaryContainer : Theme.colors.colorOnSecondaryContainer

    implicitHeight: 56
    implicitWidth: content.implicitWidth + 16 + 16

    Rectangle {
        id: background
        objectName: "background"
        anchors.fill: pillButton
        radius: pillButton.height / 2
        color: pillButton.checked ? Theme.colors.primaryContainer : Theme.colors.secondaryContainer
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
            visible: pillButton.showsChevron
            rotation: pillButton.checked ? 180 : 0

            Behavior on rotation {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.OutQuad
                }
            }
        }
    }
}
