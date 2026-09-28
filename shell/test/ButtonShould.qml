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

    Button {
        id: button
        text: "Rescan network"
    }

    TestCase {
        id: buttonTest
        name: "ButtonShould"
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: button
            signalName: "clicked"
        }

        function init() {
            button.variant = Button.Outlined;
            button.iconSource = "";
            button.busy = false;
            button.enabled = true;
            clickedSpy.clear();
        }

        /**
         * Gives the child of the button with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = buttonTest.findChild(button, objectName);
            buttonTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the button shows its text as label.
         */
        function test_show_the_text() {
            buttonTest.compare(buttonTest.child("label").text, "Rescan network");
        }

        /**
         * Tests that clicking the button emits clicked.
         */
        function test_emit_clicked_when_clicked() {
            buttonTest.mouseClick(button);
            buttonTest.compare(clickedSpy.count, 1);
        }

        /**
         * Tests that a busy button emits no clicked.
         */
        function test_ignore_clicks_while_busy() {
            button.busy = true;
            buttonTest.mouseClick(button);
            buttonTest.compare(clickedSpy.count, 0);
            button.busy = false;
            buttonTest.mouseClick(button);
            buttonTest.compare(clickedSpy.count, 1);
        }

        /**
         * Tests that a disabled button is dimmed and emits no clicked.
         */
        function test_ignore_clicks_and_dim_when_disabled() {
            button.enabled = false;
            buttonTest.mouseClick(button);
            buttonTest.compare(clickedSpy.count, 0);
            buttonTest.compare(button.opacity, Theme.disabledOpacity);
        }

        /**
         * Tests that the icon is only shown when an icon source is set.
         */
        function test_show_the_icon_only_when_set() {
            buttonTest.compare(buttonTest.child("icon").visible, false);
            button.iconSource = "qrc:/qt/qml/Blabby/Shell/icons/material/refresh.svg";
            buttonTest.compare(buttonTest.child("icon").visible, true);
        }

        /**
         * Tests that a busy button shows a busy indicator in place of the icon.
         */
        function test_show_a_busy_indicator_while_busy() {
            button.iconSource = "qrc:/qt/qml/Blabby/Shell/icons/material/refresh.svg";
            buttonTest.compare(buttonTest.child("busyIndicator").visible, false);
            button.busy = true;
            buttonTest.compare(buttonTest.child("busyIndicator").visible, true);
            buttonTest.compare(buttonTest.child("busyIndicator").running, true);
            buttonTest.compare(buttonTest.child("icon").visible, false);
        }

        /**
         * The variants with their expected container, outline and label colours.
         */
        function test_style_the_variant_data() {
            return [
                {
                    tag: "outlined",
                    variant: Button.Outlined,
                    container: "transparent",
                    outlined: true,
                    label: Theme.colors.primary
                },
                {
                    tag: "text",
                    variant: Button.Text,
                    container: "transparent",
                    outlined: false,
                    label: Theme.colors.primary
                },
                {
                    tag: "filled",
                    variant: Button.Filled,
                    container: Theme.colors.primary,
                    outlined: false,
                    label: Theme.colors.colorOnPrimary
                }
            ];
        }

        /**
         * Tests that the outlined, text and filled variants are drawn in their colours.
         */
        function test_style_the_variant(data) {
            button.variant = data.variant;
            const container = buttonTest.child("container");
            buttonTest.verify(Qt.colorEqual(container.color, data.container));
            buttonTest.compare(container.border.width > 0, data.outlined);
            buttonTest.verify(Qt.colorEqual(buttonTest.child("label").color, data.label));
        }
    }
}
