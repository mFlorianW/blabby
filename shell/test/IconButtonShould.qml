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

    IconButton {
        id: iconButton
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/delete.svg"
    }

    TestCase {
        id: iconButtonTest
        name: "IconButtonShould"
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: iconButton
            signalName: "clicked"
        }

        function init() {
            iconButton.variant = IconButton.Standard;
            iconButton.enabled = true;
            clickedSpy.clear();
        }

        /**
         * Gives the child of the icon button with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = iconButtonTest.findChild(iconButton, objectName);
            iconButtonTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the icon button shows its icon.
         */
        function test_show_the_icon() {
            iconButtonTest.compare(iconButtonTest.child("icon").source, iconButton.iconSource);
        }

        /**
         * Tests that the icon button has the touch target and the container size of the design.
         */
        function test_have_the_size_of_the_design() {
            iconButtonTest.compare(iconButton.width, 48);
            iconButtonTest.compare(iconButton.height, 48);
            iconButtonTest.compare(iconButtonTest.child("container").width, 40);
            iconButtonTest.compare(iconButtonTest.child("icon").width, 24);
        }

        /**
         * Tests that clicking the icon button emits clicked.
         */
        function test_emit_clicked_when_clicked() {
            iconButtonTest.mouseClick(iconButton);
            iconButtonTest.compare(clickedSpy.count, 1);
        }

        /**
         * Tests that a disabled icon button is dimmed and emits no clicked.
         */
        function test_ignore_clicks_and_dim_when_disabled() {
            iconButton.enabled = false;
            iconButtonTest.mouseClick(iconButton);
            iconButtonTest.compare(clickedSpy.count, 0);
            iconButtonTest.compare(iconButton.opacity, Theme.disabledOpacity);
        }

        /**
         * The variants with their expected container and icon colours.
         */
        function test_style_the_variant_data() {
            return [
                {
                    tag: "standard",
                    variant: IconButton.Standard,
                    container: "transparent",
                    icon: Theme.colors.colorOnSurfaceVariant
                },
                {
                    tag: "tonal",
                    variant: IconButton.Tonal,
                    container: Theme.colors.secondaryContainer,
                    icon: Theme.colors.colorOnSecondaryContainer
                }
            ];
        }

        /**
         * Tests that the standard and tonal variants are drawn in their Theme colours.
         */
        function test_style_the_variant(data) {
            iconButton.variant = data.variant;
            iconButtonTest.verify(Qt.colorEqual(iconButtonTest.child("container").color, data.container));
            iconButtonTest.verify(Qt.colorEqual(iconButtonTest.child("icon").color, data.icon));
        }
    }
}
