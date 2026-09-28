// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 100
    height: 100

    BusyIndicator {
        id: busyIndicator
    }

    TestCase {
        id: busyIndicatorTest
        name: "BusyIndicatorShould"
        when: windowShown

        function init() {
            busyIndicator.running = false;
        }

        /**
         * Tests that the indicator is only shown while it is running.
         */
        function test_show_only_while_running() {
            busyIndicatorTest.compare(busyIndicator.visible, false);
            busyIndicator.running = true;
            busyIndicatorTest.compare(busyIndicator.visible, true);
        }
    }
}
