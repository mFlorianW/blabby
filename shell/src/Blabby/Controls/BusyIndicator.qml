// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Shapes

/**
 * A spinning arc that shows that an operation is running.
 */
Item {
    id: busyIndicator

    /**
     * True to show and spin the indicator.
     */
    property bool running: false

    /**
     * The colour of the arc, should be a Theme colour role.
     */
    property color color

    /**
     * The width of the arc.
     */
    property real strokeWidth: 2

    implicitWidth: 24
    implicitHeight: 24
    visible: busyIndicator.running

    Shape {
        id: arc
        anchors.fill: busyIndicator

        ShapePath {
            fillColor: "transparent"
            strokeColor: busyIndicator.color
            strokeWidth: busyIndicator.strokeWidth
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: arc.width / 2
                centerY: arc.height / 2
                radiusX: (arc.width - busyIndicator.strokeWidth) / 2
                radiusY: (arc.height - busyIndicator.strokeWidth) / 2
                startAngle: 0
                sweepAngle: 270
            }
        }

        RotationAnimator on rotation {
            from: 0
            to: 360
            duration: 1000
            loops: Animation.Infinite
            running: busyIndicator.running
        }
    }
}
