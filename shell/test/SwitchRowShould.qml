// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Theme

Item {
    id: root
    width: 600
    height: 200

    SwitchRow {
        id: switchRow
        width: root.width
        title: "Remember last Renderer"
        subtitle: "Make the Renderer you last picked active again when it is Online"
    }

    TestCase {
        id: switchRowTest
        name: "SwitchRowShould"
        when: windowShown

        SignalSpy {
            id: toggledSpy
            target: switchRow
            signalName: "toggled"
        }

        function init() {
            switchRow.subtitle = "Make the Renderer you last picked active again when it is Online";
            switchRow.checked = false;
            toggledSpy.clear();
        }

        /**
         * Gives the child of the switch row with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = switchRowTest.findChild(switchRow, objectName);
            switchRowTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the switch row shows its title and its subtitle, with the switch at its trailing edge.
         */
        function test_show_the_title_and_the_subtitle_before_the_switch() {
            const title = switchRowTest.child("title");
            const subtitle = switchRowTest.child("subtitle");
            const track = switchRowTest.child("track");
            switchRowTest.compare(title.text, "Remember last Renderer");
            switchRowTest.compare(subtitle.text, "Make the Renderer you last picked active again when it is Online");
            switchRowTest.compare(subtitle.visible, true);
            switchRowTest.verify(track.mapToItem(switchRow, 0, 0).x >= title.mapToItem(switchRow, title.width, 0).x);
            switchRowTest.verify(track.mapToItem(switchRow, track.width, 0).x <= switchRow.width);
        }

        /**
         * Tests that the switch row hides an empty subtitle.
         */
        function test_hide_an_empty_subtitle() {
            switchRow.subtitle = "";
            switchRowTest.compare(switchRowTest.child("subtitle").visible, false);
        }

        /**
         * Tests that the switch row is a large enough touch target.
         */
        function test_be_a_large_enough_touch_target() {
            switchRowTest.verify(switchRow.height >= 44);
        }

        /**
         * Tests that a switch that is off has an outlined track with a small handle at its leading edge.
         */
        function test_show_the_off_look_while_unchecked() {
            const track = switchRowTest.child("track");
            const handle = switchRowTest.child("handle");
            switchRowTest.verify(Qt.colorEqual(track.color, Theme.colors.surfaceContainerHighest));
            switchRowTest.verify(Qt.colorEqual(track.border.color, Theme.colors.outline));
            switchRowTest.verify(Qt.colorEqual(handle.color, Theme.colors.outline));
            switchRowTest.compare(handle.width, 16);
            switchRowTest.tryVerify(() => handle.x + handle.width / 2 < track.width / 2);
        }

        /**
         * Tests that a switch that is on has a filled track with a large handle at its trailing edge.
         */
        function test_show_the_on_look_while_checked() {
            switchRow.checked = true;
            const track = switchRowTest.child("track");
            const handle = switchRowTest.child("handle");
            switchRowTest.verify(Qt.colorEqual(track.color, Theme.colors.primary));
            switchRowTest.verify(Qt.colorEqual(handle.color, Theme.colors.colorOnPrimary));
            switchRowTest.compare(handle.width, 24);
            switchRowTest.tryVerify(() => handle.x + handle.width / 2 > track.width / 2);
        }

        /**
         * Tests that tapping the row emits toggled with the opposite value and leaves checked to the owner.
         */
        function test_emit_toggled_with_the_opposite_value_when_tapped() {
            switchRowTest.mouseClick(switchRow);
            switchRowTest.compare(toggledSpy.count, 1);
            switchRowTest.compare(toggledSpy.signalArguments[0][0], true);
            switchRowTest.compare(switchRow.checked, false);
            switchRow.checked = true;
            switchRowTest.mouseClick(switchRowTest.child("track"));
            switchRowTest.compare(toggledSpy.count, 2);
            switchRowTest.compare(toggledSpy.signalArguments[1][0], false);
        }
    }
}
