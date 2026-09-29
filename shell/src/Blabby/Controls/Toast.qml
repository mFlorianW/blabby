// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A short transient message, e.g. that something failed, that hides itself after a short time.
 * The toast is hidden until @ref show() is called, a new message replaces the shown one.
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
     * Shows the message for the duration, replacing a shown message.
     */
    function show(message: string) {
        messageText.text = message;
        hideTimer.restart();
    }

    /**
     * Hides the toast right away.
     */
    function hide() {
        hideTimer.stop();
    }

    implicitWidth: Math.min(messageText.implicitWidth + 32, toast.maximumWidth)
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
        anchors.right: toast.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        elide: Text.ElideRight
        color: Theme.colors.colorOnSurface
        textStyle: Theme.fonts.bodyMedium
    }
}
