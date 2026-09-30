// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 600
    height: 200

    Slider {
        id: slider
        anchors.centerIn: parent
        width: 400
        from: 0
        to: 100
        value: 20
        valueIndicatorText: "1:23"
    }

    TestCase {
        id: sliderTest
        name: "SliderShould"
        when: windowShown

        SignalSpy {
            id: movedSpy
            target: slider
            signalName: "moved"
        }

        SignalSpy {
            id: committedSpy
            target: slider
            signalName: "committed"
        }

        function init() {
            slider.value = 20;
            slider.interactive = true;
            slider.valueIndicatorEnabled = false;
            movedSpy.clear();
            committedSpy.clear();
        }

        /**
         * Gives the child of the slider with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = sliderTest.findChild(slider, objectName);
            sliderTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the slider shows its value and is a big enough touch target.
         */
        function test_show_the_value() {
            sliderTest.compare(slider.visualPosition, 0.2);
            sliderTest.verify(slider.height >= 44);
            const thumb = sliderTest.child("thumb");
            sliderTest.verify(Math.abs(thumb.x + thumb.width / 2 - 0.2 * slider.width) <= thumb.width);
        }

        /**
         * Tests that the thumb follows the finger while dragging and the value is committed once on release.
         */
        function test_follow_the_finger_and_commit_on_release() {
            sliderTest.mousePress(slider, 100, slider.height / 2);
            sliderTest.mouseMove(slider, 300, slider.height / 2);
            sliderTest.compare(slider.pressed, true);
            sliderTest.compare(slider.visualValue, 75);
            sliderTest.verify(movedSpy.count > 0);
            sliderTest.compare(movedSpy.signalArguments[movedSpy.count - 1][0], 75);
            sliderTest.compare(committedSpy.count, 0);
            sliderTest.mouseRelease(slider, 300, slider.height / 2);
            sliderTest.compare(committedSpy.count, 1);
            sliderTest.compare(committedSpy.signalArguments[0][0], 75);
        }

        /**
         * Tests that a tap commits the value at that point.
         */
        function test_commit_the_value_of_a_tap() {
            sliderTest.mouseClick(slider, 200, slider.height / 2);
            sliderTest.compare(committedSpy.count, 1);
            sliderTest.compare(committedSpy.signalArguments[0][0], 50);
        }

        /**
         * Tests that value changes are ignored while the finger is on the slider.
         */
        function test_ignore_value_changes_while_pressed() {
            sliderTest.mousePress(slider, 300, slider.height / 2);
            slider.value = 10;
            sliderTest.compare(slider.visualValue, 75);
            sliderTest.mouseRelease(slider, 300, slider.height / 2);
            sliderTest.compare(slider.visualValue, 10);
        }

        /**
         * Tests that the value indicator is only shown while pressed.
         */
        function test_show_the_value_indicator_while_pressed() {
            slider.valueIndicatorEnabled = true;
            const indicator = sliderTest.child("valueIndicator");
            sliderTest.compare(indicator.visible, false);
            sliderTest.mousePress(slider, 300, slider.height / 2);
            sliderTest.compare(indicator.visible, true);
            sliderTest.compare(sliderTest.findChild(indicator, "valueIndicatorText").text, "1:23");
            sliderTest.verify(indicator.y + indicator.height <= 0);
            sliderTest.mouseRelease(slider, 300, slider.height / 2);
            sliderTest.compare(indicator.visible, false);
        }

        /**
         * Tests that the value indicator isn't shown when it isn't enabled.
         */
        function test_show_no_value_indicator_when_not_enabled() {
            sliderTest.mousePress(slider, 300, slider.height / 2);
            sliderTest.compare(sliderTest.child("valueIndicator").visible, false);
            sliderTest.mouseRelease(slider, 300, slider.height / 2);
        }

        /**
         * Tests that a slider that isn't interactive only shows its value.
         */
        function test_ignore_the_finger_when_not_interactive() {
            slider.interactive = false;
            sliderTest.mousePress(slider, 300, slider.height / 2);
            sliderTest.compare(slider.pressed, false);
            sliderTest.mouseRelease(slider, 300, slider.height / 2);
            sliderTest.compare(movedSpy.count, 0);
            sliderTest.compare(committedSpy.count, 0);
            sliderTest.compare(slider.visualValue, 20);
        }
    }
}
