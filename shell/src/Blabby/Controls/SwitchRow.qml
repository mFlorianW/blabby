// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A row with a title, an optional subtitle and a switch at its trailing edge that turns a setting on or off.
 * Tapping anywhere on the row emits toggled with the opposite value. The row doesn't flip checked itself, its owner
 * stores the new value and passes it back in checked.
 */
AbstractInteractiveControl {
    id: switchRow

    /**
     * The title of the setting.
     */
    property string title

    /**
     * A short description shown below the title, hidden when empty.
     */
    property string subtitle

    /**
     * True while the switch is on, false by default.
     */
    property bool checked: false

    /**
     * This signal is emitted when the user taps the row, checked is the value the user asks for.
     */
    signal toggled(bool checked)

    implicitHeight: 72
    onClicked: switchRow.toggled(!switchRow.checked)

    Rectangle {
        id: stateLayer
        anchors.fill: switchRow
        radius: 16
        color: Theme.colors.colorOnSurface
        opacity: switchRow.pressed ? Theme.stateLayer.pressedOpacity : switchRow.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    Column {
        id: texts
        anchors.left: switchRow.left
        anchors.leftMargin: 12
        anchors.right: track.left
        anchors.rightMargin: 16
        anchors.verticalCenter: switchRow.verticalCenter

        StyledText {
            id: titleText
            objectName: "title"
            width: texts.width
            text: switchRow.title
            color: Theme.colors.colorOnSurface
            textStyle: Theme.fonts.bodyLarge
        }

        StyledText {
            id: subtitleText
            objectName: "subtitle"
            width: texts.width
            text: switchRow.subtitle
            color: Theme.colors.colorOnSurfaceVariant
            textStyle: Theme.fonts.bodyMedium
            visible: switchRow.subtitle !== ""
        }
    }

    Rectangle {
        id: track
        objectName: "track"
        anchors.right: switchRow.right
        anchors.rightMargin: 12
        anchors.verticalCenter: switchRow.verticalCenter
        width: 52
        height: 32
        radius: track.height / 2
        color: switchRow.checked ? Theme.colors.primary : Theme.colors.surfaceContainerHighest
        border.width: switchRow.checked ? 0 : 2
        border.color: Theme.colors.outline

        Rectangle {
            id: handle
            objectName: "handle"
            // The handle grows while pressed, and is larger while the switch is on.
            width: switchRow.pressed ? 28 : switchRow.checked ? 24 : 16
            height: handle.width
            radius: handle.width / 2
            // The centre of the handle sits half a track height from the edge it rests at.
            x: (switchRow.checked ? track.width - track.height / 2 : track.height / 2) - handle.width / 2
            anchors.verticalCenter: track.verticalCenter
            color: switchRow.checked ? Theme.colors.colorOnPrimary : Theme.colors.outline

            Behavior on x {
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.OutQuad
                }
            }
        }
    }
}
