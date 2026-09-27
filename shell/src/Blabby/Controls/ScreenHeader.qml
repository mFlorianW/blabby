// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Theme

/**
 * The header at the top of a screen with a title, a subtitle below it and a trailing slot for actions.
 * Items declared inside the header are placed in the trailing slot.
 */
Item {
    id: header

    /**
     * The title of the screen.
     */
    property string title

    /**
     * A short description shown below the title.
     */
    property string subtitle

    /**
     * The actions shown at the trailing edge of the header.
     */
    default property alias actions: actionRow.data

    implicitHeight: 64

    Column {
        id: titleColumn
        anchors.left: header.left
        anchors.right: actionRow.left
        anchors.rightMargin: 16
        anchors.verticalCenter: header.verticalCenter

        StyledText {
            id: titleText
            objectName: "title"
            width: titleColumn.width
            text: header.title
            color: Theme.colors.colorOnSurface
            textStyle: Theme.fonts.headlineMedium
        }

        StyledText {
            id: subtitleText
            objectName: "subtitle"
            width: titleColumn.width
            text: header.subtitle
            color: Theme.colors.colorOnSurfaceVariant
            textStyle: Theme.fonts.bodyMedium
        }
    }

    Row {
        id: actionRow
        anchors.right: header.right
        anchors.verticalCenter: header.verticalCenter
        spacing: 8
    }
}
