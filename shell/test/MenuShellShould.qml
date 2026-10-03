// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 800
    height: 480

    MouseArea {
        id: beneath
        objectName: "beneath"
        anchors.fill: root
        hoverEnabled: true
    }

    Rectangle {
        id: anchor
        x: 500
        y: 40
        width: 120
        height: 40
    }

    MenuShell {
        id: menu
        anchors.fill: root
        anchorItem: anchor
        panelWidth: 300

        Rectangle {
            id: content
            objectName: "content"
            width: menu.contentWidth
            height: 100
        }
    }

    MenuShell {
        id: tallMenu
        anchors.fill: root
        anchorItem: anchor

        Rectangle {
            id: tallContent
            width: tallMenu.contentWidth
            height: 1000
        }
    }

    TestCase {
        id: menuShellTest
        name: "MenuShellShould"
        when: windowShown

        SignalSpy {
            id: beneathClickedSpy
            target: beneath
            signalName: "clicked"
        }

        SignalSpy {
            id: beneathWheelSpy
            target: beneath
            signalName: "wheel"
        }

        function init() {
            anchor.x = 500;
            anchor.y = 40;
            tallMenu.close();
            tallMenu.opensUpwards = false;
            menu.opensUpwards = false;
            menu.visible = true;
            menu.close();
            beneathClickedSpy.clear();
            beneathWheelSpy.clear();
        }

        /**
         * Gives the child of the menu with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = menuShellTest.findChild(menu, objectName);
            menuShellTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the menu is closed at start, with neither the panel nor the tap catcher shown.
         */
        function test_be_closed_at_start() {
            menuShellTest.compare(menu.opened, false);
            menuShellTest.compare(menu.panel.visible, false);
            menuShellTest.compare(menuShellTest.child("tapCatcher").visible, false);
        }

        /**
         * Tests that open() shows the panel with the content and close() hides it again.
         */
        function test_open_and_close() {
            menu.open();
            menuShellTest.compare(menu.opened, true);
            menuShellTest.compare(menu.panel.visible, true);
            menuShellTest.compare(menuShellTest.child("content").visible, true);
            menu.close();
            menuShellTest.compare(menu.opened, false);
            menuShellTest.compare(menu.panel.visible, false);
        }

        /**
         * Tests that the panel drops down 8 px under the anchor item, aligned to its right edge, and wraps the
         * content with 8 px above and below.
         */
        function test_drop_down_under_the_bottom_right_corner_of_the_anchor_item() {
            menu.open();
            const panelTopRight = menu.panel.mapToItem(root, menu.panel.width, 0);
            menuShellTest.compare(panelTopRight.x, anchor.x + anchor.width);
            menuShellTest.compare(panelTopRight.y, anchor.y + anchor.height + 8);
            menuShellTest.compare(menu.panel.width, 300);
            menuShellTest.compare(menu.panel.height, 116);
            const contentTopLeft = content.mapToItem(menu.panel, 0, 0);
            menuShellTest.compare(contentTopLeft.y, 8);
        }

        /**
         * Tests that the panel opens upwards 8 px above the anchor item, aligned to its right edge.
         */
        function test_open_upwards_above_the_top_right_corner_of_the_anchor_item() {
            anchor.y = 400;
            menu.opensUpwards = true;
            menu.open();
            const panelBottomRight = menu.panel.mapToItem(root, menu.panel.width, menu.panel.height);
            menuShellTest.compare(panelBottomRight.x, anchor.x + anchor.width);
            menuShellTest.compare(panelBottomRight.y, anchor.y - 8);
            menuShellTest.compare(menu.panel.height, 116);
        }

        /**
         * Tests that the content may take the space towards the edge the menu opens to, less the gaps to the anchor
         * item and the edge and around the content.
         */
        function test_tell_the_content_the_space_towards_the_edge() {
            menuShellTest.compare(menu.maximumContentHeight, 480 - 80 - 4 * 8);
            anchor.y = 400;
            menu.opensUpwards = true;
            menuShellTest.compare(menu.maximumContentHeight, 400 - 4 * 8);
        }

        /**
         * Tests that the panel of content higher than the space is capped 8 px before the edge and clips the content.
         */
        function test_cap_the_panel_to_the_space_towards_the_edge() {
            tallMenu.open();
            const panelBottom = tallMenu.panel.mapToItem(root, 0, tallMenu.panel.height).y;
            menuShellTest.compare(panelBottom, root.height - 8);
            anchor.y = 400;
            tallMenu.opensUpwards = true;
            menuShellTest.compare(tallMenu.panel.mapToItem(root, 0, 0).y, 8);
            menuShellTest.compare(tallContent.parent.parent.clip, true);
        }

        /**
         * Tests that the panel follows the anchor item when it moves.
         */
        function test_follow_the_anchor_item_when_it_moves() {
            menu.open();
            anchor.x = 300;
            const panelTopRight = menu.panel.mapToItem(root, menu.panel.width, 0);
            menuShellTest.compare(panelTopRight.x, 420);
        }

        /**
         * Tests that the panel has a rounded background with a shadow.
         */
        function test_draw_a_rounded_background_with_a_shadow() {
            menu.open();
            const background = menuShellTest.child("panelBackground");
            menuShellTest.compare(background.radius, 16);
            menuShellTest.compare(menuShellTest.child("panelShadow").shadowEnabled, true);
        }

        /**
         * Tests that a tap outside the panel closes the menu without reaching anything beneath, there is no scrim.
         */
        function test_close_with_a_tap_outside_without_reaching_beneath() {
            menu.open();
            const tapCatcher = menuShellTest.child("tapCatcher");
            menuShellTest.compare(tapCatcher.width, root.width);
            menuShellTest.compare(tapCatcher.height, root.height);
            menuShellTest.compare(menuShellTest.findChild(menu, "scrim"), null);
            menuShellTest.mouseClick(root, 50, 400);
            menuShellTest.compare(menu.opened, false);
            menuShellTest.compare(beneathClickedSpy.count, 0);
        }

        /**
         * Tests that a tap on the panel outside of the content neither closes the menu nor reaches anything beneath.
         */
        function test_keep_open_with_a_tap_on_the_panel() {
            menu.open();
            menuShellTest.mouseClick(menu.panel, 10, menu.panel.height - 4);
            menuShellTest.compare(menu.opened, true);
            menuShellTest.compare(beneathClickedSpy.count, 0);
        }

        /**
         * Tests that neither the hover nor the wheel reach anything beneath the open menu.
         */
        function test_swallow_hover_and_wheel_while_open() {
            menu.open();
            menuShellTest.mouseMove(root, 50, 400);
            menuShellTest.compare(beneath.containsMouse, false);
            menuShellTest.mouseWheel(root, 50, 400, 0, -120);
            menuShellTest.compare(beneathWheelSpy.count, 0);
        }

        /**
         * Tests that the menu closes when it becomes invisible and stays closed when it is shown again.
         */
        function test_close_when_invisible() {
            menu.open();
            menu.visible = false;
            menuShellTest.compare(menu.opened, false);
            anchor.x = 500;
            anchor.y = 40;
            tallMenu.close();
            tallMenu.opensUpwards = false;
            menu.opensUpwards = false;
            menu.visible = true;
            menuShellTest.compare(menu.opened, false);
            menuShellTest.compare(menu.panel.visible, false);
        }
    }
}
