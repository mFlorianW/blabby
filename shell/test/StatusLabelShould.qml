// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Theme

Item {
    id: root
    width: 200
    height: 200

    StatusLabel {
        id: statusLabel
        text: "Playing"
    }

    TestCase {
        id: statusLabelTest
        name: "StatusLabelShould"
        when: windowShown

        function init() {
            statusLabel.iconSource = "";
            statusLabel.emphasis = false;
        }

        /**
         * Tests that the label shows the given text.
         */
        function test_show_the_text() {
            statusLabelTest.compare(statusLabelTest.findChild(statusLabel, "label").text, "Playing");
        }

        /**
         * Tests that the icon is only shown when an icon source is set.
         */
        function test_show_the_icon_only_when_set() {
            const icon = statusLabelTest.findChild(statusLabel, "icon");
            statusLabelTest.verify(icon);
            statusLabelTest.compare(icon.visible, false);
            statusLabel.iconSource = "qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg";
            statusLabelTest.compare(icon.visible, true);
        }

        /**
         * Tests that an emphasised label draws icon and text in the primary colour.
         */
        function test_draw_the_emphasised_label_in_the_primary_colour() {
            const icon = statusLabelTest.findChild(statusLabel, "icon");
            const label = statusLabelTest.findChild(statusLabel, "label");
            statusLabelTest.verify(Qt.colorEqual(label.color, Theme.colors.colorOnSurfaceVariant));
            statusLabel.emphasis = true;
            statusLabelTest.verify(Qt.colorEqual(label.color, Theme.colors.primary));
            statusLabelTest.verify(Qt.colorEqual(icon.color, Theme.colors.primary));
        }
    }
}
