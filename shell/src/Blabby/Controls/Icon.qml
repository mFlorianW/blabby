// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects

/**
 * Displays a monochrome SVG icon recoloured to the given colour.
 * The alpha channel of the SVG is used as mask, so the colours inside the SVG are ignored.
 */
Item {
    id: icon

    /**
     * The URL of the SVG icon.
     */
    property url source

    /**
     * The colour the icon is painted in, should be a Theme colour role.
     */
    property color color

    implicitWidth: 24
    implicitHeight: 24

    Image {
        id: mask
        anchors.fill: icon
        source: icon.source
        sourceSize.width: icon.width
        sourceSize.height: icon.height
        visible: false
        layer.enabled: true
    }

    Rectangle {
        id: fill
        anchors.fill: icon
        color: icon.color
        visible: false
        layer.enabled: true
    }

    MultiEffect {
        anchors.fill: icon
        source: fill
        maskEnabled: true
        maskSource: mask
        // Maps the mask alpha linearly to the opacity to keep the anti-aliased edges of the SVG.
        maskThresholdMin: 0.5
        maskSpreadAtMin: 1.0
    }
}
