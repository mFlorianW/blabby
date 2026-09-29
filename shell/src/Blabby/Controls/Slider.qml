// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A slider to pick a value between from and to by dragging or tapping.
 * While the finger is on the slider, the thumb follows the finger and changes of value are ignored; @ref moved is
 * emitted for every move and @ref committed once on release, also for a tap. An optional value indicator bubble above
 * the thumb shows the valueIndicatorText while pressed. A slider that isn't interactive only shows its value.
 */
Item {
    id: slider

    /**
     * The value at the left end of the slider.
     */
    property real from: 0

    /**
     * The value at the right end of the slider.
     */
    property real to: 1

    /**
     * The value shown while the finger isn't on the slider.
     */
    property real value: 0

    /**
     * False to only show the value and ignore the finger.
     */
    property bool interactive: true

    /**
     * True to show the value indicator bubble above the thumb while pressed.
     */
    property bool valueIndicatorEnabled: false

    /**
     * The text of the value indicator, e.g. the formatted pressedValue.
     */
    property string valueIndicatorText

    /**
     * True while the finger is on the slider.
     */
    readonly property bool pressed: mouseArea.pressed

    /**
     * The value under the finger while pressed.
     */
    readonly property real pressedValue: mouseArea.pressedValue

    /**
     * The value the slider shows: the value under the finger while pressed, otherwise value.
     */
    readonly property real visualValue: slider.pressed ? slider.pressedValue : slider.value

    /**
     * The position of the visual value between 0 (from) and 1 (to).
     */
    readonly property real visualPosition: slider.to > slider.from ? Math.max(0, Math.min(1, (slider.visualValue - slider.from) / (slider.to - slider.from))) : 0

    /**
     * This signal is emitted while the finger moves the slider.
     */
    signal moved(real value)

    /**
     * This signal is emitted when the finger leaves the slider, also after a tap.
     */
    signal committed(real value)

    implicitWidth: 200
    implicitHeight: 44

    // The track leaves a gap on both sides of the thumb, as in the design.
    readonly property real thumbGap: 6
    readonly property real thumbX: slider.visualPosition * (slider.width - thumb.width)

    Rectangle {
        id: activeTrack
        objectName: "activeTrack"
        anchors.verticalCenter: slider.verticalCenter
        x: 0
        width: Math.max(0, slider.thumbX - slider.thumbGap)
        height: 16
        radius: 8
        color: Theme.colors.primary
    }

    Rectangle {
        id: inactiveTrack
        objectName: "inactiveTrack"
        anchors.verticalCenter: slider.verticalCenter
        x: slider.thumbX + thumb.width + slider.thumbGap
        width: Math.max(0, slider.width - inactiveTrack.x)
        height: 16
        radius: 8
        color: Theme.colors.secondaryContainer
    }

    Rectangle {
        id: thumb
        objectName: "thumb"
        anchors.verticalCenter: slider.verticalCenter
        x: slider.thumbX
        width: 4
        height: 44
        radius: 2
        color: Theme.colors.primary
    }

    Rectangle {
        id: valueIndicator
        objectName: "valueIndicator"
        x: Math.max(0, Math.min(slider.width - valueIndicator.width, thumb.x + thumb.width / 2 - valueIndicator.width / 2))
        y: -valueIndicator.height - 8
        width: Math.max(48, valueIndicatorLabel.implicitWidth + 24)
        height: 44
        radius: 22
        color: Theme.colors.colorOnSurface
        visible: slider.valueIndicatorEnabled && slider.pressed

        StyledText {
            id: valueIndicatorLabel
            objectName: "valueIndicatorText"
            anchors.centerIn: valueIndicator
            text: slider.valueIndicatorText
            color: Theme.colors.surface
            textStyle: Theme.fonts.labelLarge
        }
    }

    MouseArea {
        id: mouseArea

        property real pressedValue: 0

        function valueAt(x: real): real {
            const position = Math.max(0, Math.min(1, x / slider.width));
            return slider.from + position * (slider.to - slider.from);
        }

        anchors.fill: slider
        enabled: slider.interactive
        preventStealing: true
        onPressed: mouse => {
            mouseArea.pressedValue = mouseArea.valueAt(mouse.x);
            slider.moved(mouseArea.pressedValue);
        }
        onPositionChanged: mouse => {
            mouseArea.pressedValue = mouseArea.valueAt(mouse.x);
            slider.moved(mouseArea.pressedValue);
        }
        onReleased: slider.committed(mouseArea.pressedValue)
    }
}
