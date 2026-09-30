// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * An icon inside a filled circle, e.g. to show the kind of a thing in a @ref Card.
 * An emphasised badge is filled with the primary colour. The icon takes half of the badge's width.
 */
Rectangle {
    id: badge

    /**
     * The URL of the SVG icon.
     */
    property url iconSource

    /**
     * True to draw the badge in the primary colour.
     */
    property bool emphasis: false

    implicitWidth: 48
    implicitHeight: 48
    radius: badge.width / 2
    color: badge.emphasis ? Theme.colors.primary : Theme.colors.surfaceContainerHighest

    Icon {
        id: icon
        objectName: "icon"
        anchors.centerIn: badge
        width: Math.round(badge.width / 2)
        height: icon.width
        source: badge.iconSource
        color: badge.emphasis ? Theme.colors.colorOnPrimary : Theme.colors.colorOnSurfaceVariant
    }
}
