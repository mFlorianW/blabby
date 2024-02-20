// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    height: 450
    width: root.height

    AbstractSlider {
        id: slider
        width: 400
        height: 20
    }

    TestCase {
        id: sliderTest
        when: windowShown
        name: "SliderTest"

        function cleanup() {
            slider.value = 0;
            slider.width = 400;
            slider.stepSize = 1;
        }

        function test_positionValueOnClick() {
            compare(slider.visualPosition, 0);
            compare(slider.value, 0);
            mouseClick(slider, 100, 0);
            compare(slider.visualPosition, 100);
            compare(slider.value, 25);
            mousePress(slider, 350, 0);
            mouseRelease(root, 402, 0);
            compare(slider.visualPosition, 400);
            compare(slider.value, 100);
        }

        function test_valueUpdates() {
            compare(slider.visualPosition, 0);
            compare(slider.value, 0);
            slider.value = 100;
            compare(slider.value, 100);
            compare(slider.visualPosition, 400);
        }

        function test_handleWidthChanges() {
            compare(slider.visualPosition, 0);
            compare(slider.value, 0);
            slider.value = 100;
            slider.width = 200;
            compare(slider.value, 100);
            compare(slider.visualPosition, 200);
        }

        function test_handleMouseMovesWhenPressed() {
            compare(slider.visualPosition, 0);
            compare(slider.value, 0);
            mousePress(slider, 0, 0);
            mouseMove(slider, 100, 0, Qt.LeftButton);
            mouseRelease(slider, 100, 0);
            compare(slider.value, 25);
            compare(slider.visualPosition, 100);
        }

        function test_handleStepsize() {
            compare(slider.visualPosition, 0);
            compare(slider.value, 0);
            mouseClick(slider, 55, 0);
            compare(slider.value, 14);
            compare(slider.visualPosition, 56);
            slider.stepSize = 25;
            compare(slider.value, 25);
            compare(slider.visualPosition, 100);
        }
    }
}
