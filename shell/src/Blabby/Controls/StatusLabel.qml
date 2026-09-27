// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A short status text with an optional leading icon, e.g. the Playback State of a Renderer.
 * An emphasised label is drawn in the primary colour.
 */
Row {
    id: statusLabel

    /**
     * The URL of the optional SVG icon in front of the text. No icon is shown when it is empty.
     */
    property url iconSource

    /**
     * The status text.
     */
    property string text

    /**
     * True to draw the label in the primary colour.
     */
    property bool emphasis: false

    /**
     * The colour of the icon and the text, depending on the emphasis.
     */
    readonly property color contentColor: statusLabel.emphasis ? Theme.colors.primary : Theme.colors.colorOnSurfaceVariant

    spacing: 4

    Icon {
        id: icon
        objectName: "icon"
        anchors.verticalCenter: statusLabel.verticalCenter
        width: 18
        height: 18
        source: statusLabel.iconSource
        color: statusLabel.contentColor
        visible: statusLabel.iconSource.toString() !== ""
    }

    StyledText {
        id: label
        objectName: "label"
        anchors.verticalCenter: statusLabel.verticalCenter
        text: statusLabel.text
        color: statusLabel.contentColor
        textStyle: Theme.fonts.labelLarge
    }
}
