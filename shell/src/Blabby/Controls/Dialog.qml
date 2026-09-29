// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A modal dialog with a title, an optional text, optional content and the actions to reject and accept it.
 * The dialog is centred on a scrim that covers the whole item, so it's usually anchored to the view it belongs to.
 * The actions and tapping the scrim close the dialog. The dialog is closed at start, open() shows it.
 */
Item {
    id: dialog

    /**
     * The title of the dialog, e.g. the question the user answers.
     */
    property string title

    /**
     * The supporting text below the title, not shown when empty.
     */
    property string text

    /**
     * The label of the action that accepts the dialog, the action is not shown when empty.
     */
    property string acceptText

    /**
     * The label of the action that rejects the dialog.
     */
    property string rejectText: qsTr("Cancel")

    /**
     * Additional content below the text, e.g. a list to pick from. The content needs a height.
     */
    default property alias content: contentSlot.data

    /**
     * The widest width of the dialog.
     */
    readonly property real maximumWidth: 560

    /**
     * The narrowest width of the dialog.
     */
    readonly property real minimumWidth: 280

    /**
     * The width available to the content, content items take it as their width.
     */
    readonly property alias contentWidth: contentSlot.width

    /**
     * This signal is emitted when the user accepts the dialog.
     */
    signal accepted

    /**
     * This signal is emitted when the user rejects the dialog, with the reject action or by tapping the scrim.
     */
    signal rejected

    /**
     * Shows the dialog.
     */
    function open() {
        dialog.visible = true;
    }

    /**
     * Closes the dialog without accepting or rejecting it, e.g. when the dialog did its job.
     */
    function close() {
        dialog.visible = false;
    }

    /**
     * Closes and accepts the dialog.
     */
    function accept() {
        dialog.visible = false;
        dialog.accepted();
    }

    /**
     * Closes and rejects the dialog.
     */
    function reject() {
        dialog.visible = false;
        dialog.rejected();
    }

    visible: false

    Rectangle {
        id: scrim
        objectName: "scrim"
        anchors.fill: dialog
        color: Theme.colors.scrim
        opacity: Theme.scrimOpacity
    }

    // Rejects the dialog when the user taps beside it and keeps taps, hovers and scrolling from the content below.
    MouseArea {
        id: scrimArea
        objectName: "scrimArea"
        anchors.fill: dialog
        hoverEnabled: true
        onClicked: dialog.reject()
        onWheel: wheel => wheel.accepted = true
    }

    Rectangle {
        id: panel
        objectName: "panel"
        anchors.centerIn: dialog
        width: Math.max(dialog.minimumWidth, Math.min(dialog.maximumWidth, dialog.width - 2 * 24))
        height: layout.implicitHeight + 2 * 24
        radius: 28
        color: Theme.colors.surfaceContainerHigh

        // Keeps taps on the dialog from reaching the scrim.
        MouseArea {
            id: panelArea
            anchors.fill: panel
        }

        Column {
            id: layout
            anchors.fill: panel
            anchors.margins: 24
            spacing: 16

            StyledText {
                id: titleText
                objectName: "title"
                width: layout.width
                text: dialog.title
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.headlineSmall
                wrapMode: Text.Wrap
                elide: Text.ElideNone
            }

            StyledText {
                id: supportingText
                objectName: "text"
                width: layout.width
                text: dialog.text
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.bodyMedium
                wrapMode: Text.Wrap
                elide: Text.ElideNone
                visible: dialog.text !== ""
            }

            Item {
                id: contentSlot
                objectName: "content"
                width: layout.width
                height: contentSlot.childrenRect.height
                visible: contentSlot.children.length > 0
            }

            Row {
                id: actions
                anchors.right: layout.right
                spacing: 8

                Button {
                    id: rejectButton
                    objectName: "rejectButton"
                    variant: Button.Text
                    text: dialog.rejectText
                    onClicked: dialog.reject()
                }

                Button {
                    id: acceptButton
                    objectName: "acceptButton"
                    variant: Button.Text
                    text: dialog.acceptText
                    visible: dialog.acceptText !== ""
                    onClicked: dialog.accept()
                }
            }
        }
    }
}
