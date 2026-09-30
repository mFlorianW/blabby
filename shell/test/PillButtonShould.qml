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
            pillButton.showsChevron = false;
            pillButton.checked = false;
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
         * Tests that the pill button shows no trailing chevron by default.
         */
        function test_show_no_chevron_by_default() {
            pillButtonTest.compare(pillButtonTest.child("dropDownIcon").visible, false);
        }

        /**
         * Tests that the pill button shows the trailing chevron when it is asked to and leaves room for it.
         */
        function test_show_the_chevron_when_enabled() {
            // The content is laid out on the next polish, e.g. after init reset the text of a previous test.
            pillButtonTest.waitForItemPolished(pillButtonTest.child("label").parent);
            const widthWithoutChevron = pillButton.width;
            pillButton.showsChevron = true;
            const chevron = pillButtonTest.child("dropDownIcon");
            pillButtonTest.compare(chevron.visible, true);
            pillButtonTest.compare(chevron.source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/expand_more.svg"));
            pillButtonTest.tryVerify(() => pillButton.width > widthWithoutChevron);
        }

        /**
         * Tests that the chevron turns by 180° while the pill button is checked and back when it isn't.
         */
        function test_turn_the_chevron_while_checked() {
            pillButton.showsChevron = true;
            const chevron = pillButtonTest.child("dropDownIcon");
            pillButtonTest.compare(chevron.rotation, 0);
            pillButton.checked = true;
            pillButtonTest.tryCompare(chevron, "rotation", 180);
            pillButton.checked = false;
            pillButtonTest.tryCompare(chevron, "rotation", 0);
        }

        /**
         * Tests that a checked pill button has the highlighted container colours and an unchecked one the tonal ones.
         */
        function test_show_the_checked_look_while_checked() {
            const background = pillButtonTest.child("background");
            const label = pillButtonTest.child("label");
            pillButtonTest.verify(Qt.colorEqual(background.color, Theme.colors.secondaryContainer));
            pillButtonTest.verify(Qt.colorEqual(label.color, Theme.colors.colorOnSecondaryContainer));
            pillButton.checked = true;
            pillButtonTest.verify(Qt.colorEqual(background.color, Theme.colors.primaryContainer));
            pillButtonTest.verify(Qt.colorEqual(label.color, Theme.colors.colorOnPrimaryContainer));
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
