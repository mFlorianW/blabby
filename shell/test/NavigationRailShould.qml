// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 200
    height: 480

    NavigationRail {
        id: rail
        height: root.height
        model: [
            {
                "text": "Renderers",
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            },
            {
                "text": "Sources",
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            }
        ]
    }

    TestCase {
        id: railTest
        name: "NavigationRailShould"
        when: windowShown

        SignalSpy {
            id: activatedSpy
            target: rail
            signalName: "activated"
        }

        function init() {
            rail.currentIndex = 0;
            activatedSpy.clear();
        }

        /**
         * Tests that the item at the current index is the only active item.
         */
        function test_mark_only_the_current_item_as_active() {
            railTest.compare(railTest.findChild(rail, "railItem0").active, true);
            railTest.compare(railTest.findChild(rail, "railItem1").active, false);
        }

        /**
         * Tests that clicking an item makes it the current and active item and emits activated.
         */
        function test_activate_a_clicked_item() {
            railTest.mouseClick(railTest.findChild(rail, "railItem1"));
            railTest.compare(rail.currentIndex, 1);
            railTest.compare(railTest.findChild(rail, "railItem0").active, false);
            railTest.compare(railTest.findChild(rail, "railItem1").active, true);
            railTest.compare(activatedSpy.count, 1);
            railTest.compare(activatedSpy.signalArguments[0][0], 1);
        }
    }
}
