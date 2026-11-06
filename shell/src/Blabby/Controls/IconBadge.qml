// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * An icon inside a filled circle, e.g. to show the kind of a thing in a @ref Card.
 * An emphasised badge is filled with the primary colour, a highlighted badge, e.g. of the active row of a menu, with the
 * highlighted container colours. The icon takes half of the badge's width. A monochrome icon is recoloured to the
 * content colour of the badge, any other image, e.g. the icon a Source provides, is shown in its own colours.
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

    /**
     * True to draw the badge in the highlighted container colours.
     */
    property bool highlighted: false

    /**
     * True to recolour the icon to the content colour of the badge, false to show the image in its own colours.
     */
    property bool monochrome: true

    implicitWidth: 48
    implicitHeight: 48
    radius: badge.width / 2
    color: badge.emphasis ? Theme.colors.primary : badge.highlighted ? Theme.colors.primaryContainer : Theme.colors.surfaceContainerHighest

    Icon {
        id: icon
        objectName: "icon"
        anchors.centerIn: badge
        width: Math.round(badge.width / 2)
        height: icon.width
        source: badge.iconSource
        color: badge.emphasis ? Theme.colors.colorOnPrimary : badge.highlighted ? Theme.colors.colorOnPrimaryContainer : Theme.colors.colorOnSurfaceVariant
        visible: badge.monochrome
    }

    Image {
        id: image
        objectName: "image"
        anchors.centerIn: badge
        width: icon.width
        height: icon.height
        source: badge.monochrome ? "" : badge.iconSource
        sourceSize.width: image.width
        sourceSize.height: image.height
        fillMode: Image.PreserveAspectFit
        visible: !badge.monochrome
    }
}
