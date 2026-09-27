// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 200
    height: 200

    NavigationRailItem {
        id: railItem
        text: "Renderers"
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
    }

    TestCase {
        id: railItemTest
        name: "NavigationRailItemShould"
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: railItem
            signalName: "clicked"
        }

        function init() {
            railItem.active = false;
            clickedSpy.clear();
        }

        /**
         * Tests that the label of the item shows the given text.
         */
        function test_show_the_label_text() {
            const label = railItemTest.findChild(railItem, "label");
            railItemTest.verify(label);
            railItemTest.compare(label.text, "Renderers");
        }

        /**
         * Tests that the active pill is only shown when the item is active.
         */
        function test_show_the_active_pill_only_when_active() {
            const pill = railItemTest.findChild(railItem, "activePill");
            railItemTest.verify(pill);
            railItemTest.compare(pill.visible, false);
            railItem.active = true;
            railItemTest.compare(pill.visible, true);
        }

        /**
         * Tests that clicking the item emits clicked.
         */
        function test_emit_clicked_when_clicked() {
            railItemTest.mouseClick(railItem);
            railItemTest.compare(clickedSpy.count, 1);
        }
    }
}
