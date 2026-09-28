// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A button with a label and an optional leading icon in the outlined, text or filled variant.
 * A busy button shows a busy indicator in place of the icon and emits no clicked signal.
 * A disabled button (enabled: false) is dimmed and emits no clicked signal.
 */
Item {
    id: button

    /**
     * The look of the button.
     */
    enum Variant {
        /**
         * Transparent with an outline, for secondary actions.
         */
        Outlined,
        /**
         * Only the label, for the least prominent actions.
         */
        Text,
        /**
         * Filled in the primary colour, for the most prominent action.
         */
        Filled
    }

    /**
     * The variant of the button, a value of Button.Variant.
     */
    property int variant: Button.Outlined

    /**
     * The label of the button.
     */
    property string text

    /**
     * The URL of the optional SVG icon in front of the label. No icon is shown when it is empty.
     */
    property url iconSource

    /**
     * True while the action of the button is running. A busy button can't be clicked.
     */
    property bool busy: false

    /**
     * The colour of the icon and the label, depending on the variant.
     */
    readonly property color contentColor: button.variant === Button.Filled ? Theme.colors.colorOnPrimary : Theme.colors.primary

    /**
     * True when an icon source is set.
     */
    readonly property bool hasIcon: button.iconSource.toString() !== ""

    /**
     * This signal is emitted when the button is clicked while it isn't busy.
     */
    signal clicked

    implicitHeight: 40
    implicitWidth: content.implicitWidth + (button.busy || button.hasIcon ? 16 : 24) + 24
    opacity: button.enabled ? 1 : Theme.disabledOpacity

    Rectangle {
        id: container
        objectName: "container"
        anchors.fill: button
        radius: button.height / 2
        color: button.variant === Button.Filled ? Theme.colors.primary : "transparent"
        border.color: Theme.colors.outline
        border.width: button.variant === Button.Outlined ? 1 : 0
    }

    Rectangle {
        id: stateLayer
        anchors.fill: container
        radius: container.radius
        color: button.contentColor
        opacity: control.pressed ? Theme.stateLayer.pressedOpacity : control.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    AbstractInteractiveControl {
        id: control
        anchors.fill: button
        enabled: !button.busy
        onClicked: button.clicked()
    }

    Row {
        id: content
        anchors.centerIn: button
        spacing: 8

        Icon {
            id: icon
            objectName: "icon"
            anchors.verticalCenter: content.verticalCenter
            width: 18
            height: 18
            source: button.iconSource
            color: button.contentColor
            visible: !button.busy && button.hasIcon
        }

        BusyIndicator {
            id: busyIndicator
            objectName: "busyIndicator"
            anchors.verticalCenter: content.verticalCenter
            width: 18
            height: 18
            color: button.contentColor
            running: button.busy
        }

        StyledText {
            id: label
            objectName: "label"
            anchors.verticalCenter: content.verticalCenter
            text: button.text
            color: button.contentColor
            textStyle: Theme.fonts.labelLarge
        }
    }
}
