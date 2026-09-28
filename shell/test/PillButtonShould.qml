// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Theme

Item {
    id: root
    width: 400
    height: 200

    PillButton {
        id: pillButton
        text: "NAS"
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
    }

    TestCase {
        id: pillButtonTest
        name: "PillButtonShould"
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: pillButton
            signalName: "clicked"
        }

        function init() {
            pillButton.text = "NAS";
            clickedSpy.clear();
        }

        /**
         * Gives the child of the pill button with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = pillButtonTest.findChild(pillButton, objectName);
            pillButtonTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the pill button shows its text and its leading icon.
         */
        function test_show_the_text_and_the_icon() {
            pillButtonTest.compare(pillButtonTest.child("label").text, "NAS");
            pillButtonTest.compare(pillButtonTest.child("icon").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"));
            pillButtonTest.compare(pillButtonTest.child("dropDownIcon").visible, true);
        }

        /**
         * Tests that the pill button is a pill in the secondary container colour and a large enough touch target.
         */
        function test_be_a_tonal_pill() {
            const background = pillButtonTest.child("background");
            pillButtonTest.compare(background.color, Theme.colors.secondaryContainer);
            pillButtonTest.compare(background.radius, pillButton.height / 2);
            pillButtonTest.verify(pillButton.height >= 44);
        }

        /**
         * Tests that the pill button grows with its text.
         */
        function test_grow_with_the_text() {
            const shortWidth = pillButton.width;
            pillButton.text = "A media server with a much longer name";
            pillButtonTest.tryVerify(() => pillButton.width > shortWidth);
        }

        /**
         * Tests that clicking the pill button emits clicked.
         */
        function test_emit_clicked_when_clicked() {
            pillButtonTest.mouseClick(pillButton);
            pillButtonTest.compare(clickedSpy.count, 1);
        }
    }
}
