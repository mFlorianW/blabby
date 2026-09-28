// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * The placeholder of an empty screen with an icon, a title, a hint and an optional action, centred in the available space.
 * A busy empty state shows a busy indicator in place of the icon, e.g. while searching.
 */
Item {
    id: emptyState

    /**
     * The URL of the SVG icon above the title.
     */
    property url iconSource

    /**
     * The title that tells why the screen is empty.
     */
    property string title

    /**
     * A hint what the user can do about it. No hint is shown when it is empty.
     */
    property string hint

    /**
     * True while the content of the screen is loaded, e.g. while searching.
     */
    property bool busy: false

    /**
     * The label of a button below the hint that lets the user resolve the empty state.
     * No button is shown when it is empty.
     */
    property string actionText

    /**
     * This signal is emitted when the user clicks the action button.
     */
    signal actionClicked

    Column {
        id: content
        anchors.centerIn: emptyState
        width: Math.min(emptyState.width - 48, 400)
        spacing: 8

        Item {
            id: leading
            anchors.horizontalCenter: content.horizontalCenter
            width: 48
            height: 48

            Icon {
                id: icon
                objectName: "icon"
                anchors.fill: leading
                source: emptyState.iconSource
                color: Theme.colors.colorOnSurfaceVariant
                visible: !emptyState.busy
            }

            BusyIndicator {
                id: busyIndicator
                objectName: "busyIndicator"
                anchors.fill: leading
                anchors.margins: 6
                strokeWidth: 4
                color: Theme.colors.primary
                running: emptyState.busy
            }
        }

        StyledText {
            id: titleText
            objectName: "title"
            width: content.width
            horizontalAlignment: Text.AlignHCenter
            text: emptyState.title
            color: Theme.colors.colorOnSurface
            textStyle: Theme.fonts.titleLarge
        }

        StyledText {
            id: hintText
            objectName: "hint"
            width: content.width
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            text: emptyState.hint
            color: Theme.colors.colorOnSurfaceVariant
            textStyle: Theme.fonts.bodyMedium
            visible: emptyState.hint !== ""
        }

        Item {
            id: actionSpacer
            width: content.width
            height: 8
            visible: actionButton.visible
        }

        Button {
            id: actionButton
            objectName: "action"
            anchors.horizontalCenter: content.horizontalCenter
            // Taller than the default button to be a large enough touch target.
            height: 48
            variant: Button.Filled
            text: emptyState.actionText
            visible: emptyState.actionText !== ""
            onClicked: emptyState.actionClicked()
        }
    }
}
