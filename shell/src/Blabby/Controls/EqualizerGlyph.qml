// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick

/**
 * The equaliser glyph, the bars of the graphic_eq icon, e.g. to mark what plays.
 * A running glyph moves its bars up and down, a glyph that doesn't run shows the bars of the icon.
 */
Item {
    id: glyph

    /**
     * True to move the bars.
     */
    property bool running: false

    /**
     * The colour of the bars, should be a Theme colour role.
     */
    property color color

    /**
     * The scale from the 24 px grid of the icon to the size of the glyph.
     */
    readonly property real unit: Math.min(glyph.width, glyph.height) / 24

    implicitWidth: 24
    implicitHeight: 24

    Repeater {
        id: bars
        // The heights of the bars of the icon on its 24 px grid, from left to right.
        model: [4, 12, 20, 12, 4]

        Rectangle {
            id: bar
            required property int index
            required property real modelData

            /**
             * The share of the full height the bar has while the glyph runs.
             */
            property real level: 0.2

            objectName: "bar" + bar.index
            x: (glyph.width - 18 * glyph.unit) / 2 + bar.index * 4 * glyph.unit
            anchors.verticalCenter: glyph.verticalCenter
            width: 2 * glyph.unit
            height: (glyph.running ? 20 * bar.level : bar.modelData) * glyph.unit
            color: glyph.color

            SequentialAnimation on level {
                running: glyph.running
                loops: Animation.Infinite

                NumberAnimation {
                    to: 1
                    // Each bar moves at its own pace, so the bars don't move together.
                    duration: 300 + (bar.index * 170) % 350
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 0.2
                    duration: 250 + (bar.index * 230) % 300
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
