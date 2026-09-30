// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A short transient message, e.g. that something failed, that hides itself after a short time.
 * The toast is hidden until @ref show() is called, a new message replaces the shown one.
 * A message can come with an action, e.g. a shortcut to resolve what the message tells, clicking it hides the toast.
 */
Item {
    id: toast

    /**
     * How long a message is shown in milliseconds.
     */
    property int duration: 4000

    /**
     * The widest the toast gets, a longer message is elided.
     */
    property real maximumWidth: 480

    /**
     * True while a message is shown, the toast fades out when it becomes false.
     */
    readonly property bool shown: hideTimer.running

    /**
     * This signal is emitted when the user clicks the action of the shown message.
     */
    signal actionClicked

    /**
     * Shows the message for the duration, replacing a shown message.
     * @param actionText The label of the action shown with the message, no action is shown when it is empty.
     */
    function show(message: string, actionText: string) {
        messageText.text = message;
        actionButton.text = actionText ?? "";
        hideTimer.restart();
    }

    /**
     * Hides the toast right away.
     */
    function hide() {
        hideTimer.stop();
    }

    implicitWidth: Math.min(messageText.implicitWidth + 32 + (actionButton.visible ? actionButton.width + 8 : 0), toast.maximumWidth)
    implicitHeight: 48
    opacity: toast.shown ? 1 : 0
    visible: toast.opacity > 0

    Behavior on opacity {
        NumberAnimation {
            duration: 150
            easing.type: Easing.OutQuad
        }
    }

    Timer {
        id: hideTimer
        interval: toast.duration
    }

    Rectangle {
        id: container
        objectName: "container"
        anchors.fill: toast
        radius: 8
        color: Theme.colors.surfaceContainerHighest
        border.color: Theme.colors.outlineVariant
        border.width: 1
    }

    StyledText {
        id: messageText
        objectName: "message"
        anchors.verticalCenter: toast.verticalCenter
        anchors.left: toast.left
        anchors.right: actionButton.visible ? actionButton.left : toast.right
        anchors.leftMargin: 16
        anchors.rightMargin: actionButton.visible ? 8 : 16
        elide: Text.ElideRight
        color: Theme.colors.colorOnSurface
        textStyle: Theme.fonts.bodyMedium
    }

    Button {
        id: actionButton
        objectName: "action"
        anchors.verticalCenter: toast.verticalCenter
        anchors.right: toast.right
        anchors.rightMargin: 4
        variant: Button.Text
        visible: actionButton.text !== ""
        onClicked: {
            toast.hide();
            toast.actionClicked();
        }
    }
}
