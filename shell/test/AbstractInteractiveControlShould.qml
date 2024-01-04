// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    height: 400
    width: root.height

    AbstractInteractiveControl {
        id: abstractControl
        anchors.fill: parent
    }

    TestCase {
        id: abstractControlTest
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: abstractControl
            signalName: "clicked"
        }

        SignalSpy {
            id: pressedSpy
            target: abstractControl
            signalName: "pressed"
        }

        /**
         * Tests that the pressed event is emitted when the control is pressed.
         */
        function test_pressed_is_emitted_when_the_control_is_pressed() {
            pressedSpy.clear();
            abstractControlTest.mousePress(abstractControl);
            compare(pressedSpy.count, 1);
            compare(abstractControl.controlState, AbstractInteractiveControl.Pressed);
            abstractControlTest.mouseRelease(abstractControl);
            compare(pressedSpy.count, 1);
        }

        /**
         * Tests that the clicked event is emitted when the switch is pressed.
         */
        function test_clicked_is_emitted_when_the_control_is_clicked() {
            clickedSpy.clear();
            abstractControlTest.mouseClick(abstractControl);
            compare(clickedSpy.count, 1);
        }

        /**
         * Tests that the hover state is correctly set when the mouse enters the control.
         */
        function test_set_hover_state_when_mouse_hovers_the_control() {
            abstractControl.controlState = AbstractInteractiveControl.Inactive;
            abstractControlTest.mouseMove(abstractControl, 1000, 1000);
            compare(abstractControl.controlState, AbstractInteractiveControl.Inactive);
            abstractControlTest.mouseMove(abstractControl);
            compare(abstractControl.controlState, AbstractInteractiveControl.Hovered);
            abstractControlTest.mouseMove(abstractControl, 1000, 1000);
            compare(abstractControl.controlState, AbstractInteractiveControl.Inactive);
        }
    }
}
