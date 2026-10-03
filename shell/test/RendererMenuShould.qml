// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Objects
import Blabby.Shell
import Blabby.Theme

Item {
    id: root
    width: 800
    height: 480

    ListModel {
        id: renderers
    }

    ListModel {
        id: noRenderers
    }

    MouseArea {
        id: beneath
        anchors.fill: root
    }

    Rectangle {
        id: anchor
        x: 500
        y: 40
        width: 120
        height: 40
    }

    RendererMenu {
        id: menu
        anchors.fill: root
        anchorItem: anchor
        model: renderers
    }

    TestCase {
        id: rendererMenuTest
        name: "RendererMenuShould"
        when: windowShown

        SignalSpy {
            id: pickedSpy
            target: menu
            signalName: "picked"
        }

        SignalSpy {
            id: beneathClickedSpy
            target: beneath
            signalName: "clicked"
        }

        function init() {
            renderers.clear();
            renderers.append({
                "name": "Kitchen",
                "availability": Renderer.Online,
                "active": false
            });
            renderers.append({
                "name": "Living Room",
                "availability": Renderer.Online,
                "active": true
            });
            renderers.append({
                "name": "Bedroom",
                "availability": Renderer.Offline,
                "active": false
            });
            menu.model = renderers;
            menu.visible = true;
            menu.opensUpwards = false;
            anchor.y = 40;
            menu.close();
            pickedSpy.clear();
            beneathClickedSpy.clear();
        }

        /**
         * Opens the menu and waits until its rows are laid out, so clicks hit.
         */
        function openMenu() {
            menu.open();
            // The rows are created on the polish after the list became visible, clicks before would miss them.
            rendererMenuTest.waitForItemPolished(rendererMenuTest.child("rendererList"));
        }

        /**
         * Gives the child of the menu with the objectName, null when it doesn't exist.
         */
        function child(objectName) {
            return rendererMenuTest.findChild(menu, objectName);
        }

        /**
         * Appends Online Renderers until there are count of them.
         */
        function fillUpTo(count) {
            while (renderers.count < count) {
                renderers.append({
                    "name": "Renderer " + renderers.count,
                    "availability": Renderer.Online,
                    "active": false
                });
            }
        }

        /**
         * Tests that the menu is closed at start.
         */
        function test_be_closed_at_start() {
            rendererMenuTest.compare(menu.opened, false);
            rendererMenuTest.compare(rendererMenuTest.child("rendererMenuPanel").visible, false);
        }

        /**
         * Tests that the menu is headed "Choose a Renderer".
         */
        function test_show_the_choose_a_renderer_header() {
            rendererMenuTest.openMenu();
            const header = rendererMenuTest.child("rendererMenuHeader");
            rendererMenuTest.verify(header);
            rendererMenuTest.compare(header.visible, true);
            rendererMenuTest.compare(header.text, "Choose a Renderer");
        }

        /**
         * Tests that every Renderer gets a 64 px row in the order of the model with only the speaker icon in a round
         * 40 px badge and the name on one line.
         */
        function test_list_every_renderer_with_its_icon_and_name() {
            rendererMenuTest.openMenu();
            const row = rendererMenuTest.child("rendererRow0");
            rendererMenuTest.verify(row);
            rendererMenuTest.compare(row.height, 64);
            const badge = rendererMenuTest.child("rendererBadge0");
            rendererMenuTest.compare(badge.width, 40);
            rendererMenuTest.compare(badge.radius, 20);
            rendererMenuTest.compare(badge.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"));
            const name = rendererMenuTest.child("rendererName0");
            rendererMenuTest.compare(name.text, "Kitchen");
            rendererMenuTest.compare(name.maximumLineCount, 1);
            rendererMenuTest.compare(name.elide, Text.ElideRight);
            rendererMenuTest.compare(rendererMenuTest.child("rendererName1").text, "Living Room");
            rendererMenuTest.compare(rendererMenuTest.child("rendererName2").text, "Bedroom");
            rendererMenuTest.verify(rendererMenuTest.child("rendererRow1").y > row.y);
            rendererMenuTest.verify(rendererMenuTest.child("rendererRow2").y > rendererMenuTest.child("rendererRow1").y);
            rendererMenuTest.compare(menu.count, 3);
        }

        /**
         * Tests that an Offline Renderer's row is dimmed like the Renderer card and doesn't react to taps.
         */
        function test_dim_offline_rows_and_ignore_taps_on_them() {
            rendererMenuTest.openMenu();
            const row = rendererMenuTest.child("rendererRow2");
            rendererMenuTest.compare(row.opacity, Theme.disabledOpacity);
            rendererMenuTest.compare(row.enabled, false);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRow0").opacity, 1);
            rendererMenuTest.mouseClick(row);
            rendererMenuTest.compare(pickedSpy.count, 0);
            rendererMenuTest.compare(menu.opened, true);
            rendererMenuTest.compare(beneathClickedSpy.count, 0);
        }

        /**
         * Tests that the Active Renderer's row has the secondary-container background and the others none.
         */
        function test_tint_the_row_of_the_active_renderer() {
            rendererMenuTest.openMenu();
            const activeBackground = rendererMenuTest.child("rendererRowBackground1");
            rendererMenuTest.verify(activeBackground);
            rendererMenuTest.compare(activeBackground.visible, true);
            rendererMenuTest.compare(activeBackground.color, Theme.colors.secondaryContainer);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRowBackground0").visible, false);
        }

        /**
         * Tests that picking an Online Renderer emits picked with its index and closes the menu.
         */
        function test_emit_picked_and_close_when_a_renderer_is_picked() {
            rendererMenuTest.openMenu();
            rendererMenuTest.mouseClick(rendererMenuTest.child("rendererRow0"));
            rendererMenuTest.compare(pickedSpy.count, 1);
            rendererMenuTest.compare(pickedSpy.signalArguments[0][0], 0);
            rendererMenuTest.compare(menu.opened, false);
        }

        /**
         * Tests that picking the Active Renderer only closes the menu.
         */
        function test_only_close_when_the_active_renderer_is_picked() {
            rendererMenuTest.openMenu();
            rendererMenuTest.mouseClick(rendererMenuTest.child("rendererRow1"));
            rendererMenuTest.compare(pickedSpy.count, 0);
            rendererMenuTest.compare(menu.opened, false);
        }

        /**
         * Tests that a tap outside the menu closes it without reaching anything beneath.
         */
        function test_close_with_a_tap_outside() {
            rendererMenuTest.openMenu();
            rendererMenuTest.mouseClick(root, 50, 400);
            rendererMenuTest.compare(menu.opened, false);
            rendererMenuTest.compare(pickedSpy.count, 0);
            rendererMenuTest.compare(beneathClickedSpy.count, 0);
        }

        /**
         * Tests that the menu closes when it becomes invisible.
         */
        function test_close_when_invisible() {
            rendererMenuTest.openMenu();
            menu.visible = false;
            rendererMenuTest.compare(menu.opened, false);
            menu.visible = true;
            rendererMenuTest.compare(rendererMenuTest.child("rendererMenuPanel").visible, false);
        }

        /**
         * Tests that a disabled "No Renderers found" line is shown without Renderers and only then.
         */
        function test_show_no_renderers_found_without_renderers() {
            rendererMenuTest.openMenu();
            const line = rendererMenuTest.child("noRenderersLine");
            rendererMenuTest.verify(line);
            rendererMenuTest.compare(line.visible, false);
            menu.model = noRenderers;
            rendererMenuTest.compare(menu.count, 0);
            rendererMenuTest.compare(line.visible, true);
            rendererMenuTest.compare(line.text, "No Renderers found");
            rendererMenuTest.compare(line.opacity, Theme.disabledOpacity);
            rendererMenuTest.mouseClick(line);
            rendererMenuTest.compare(menu.opened, true);
        }

        /**
         * Tests that Renderers appearing, going Offline or coming back Online while the menu is open update in it.
         */
        function test_follow_renderers_changing_while_open() {
            rendererMenuTest.openMenu();
            renderers.append({
                "name": "Office",
                "availability": Renderer.Online,
                "active": false
            });
            rendererMenuTest.tryVerify(() => rendererMenuTest.child("rendererName3") !== null);
            rendererMenuTest.compare(rendererMenuTest.child("rendererName3").text, "Office");
            renderers.setProperty(0, "availability", Renderer.Offline);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRow0").enabled, false);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRow0").opacity, Theme.disabledOpacity);
            renderers.setProperty(2, "availability", Renderer.Online);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRow2").enabled, true);
            rendererMenuTest.compare(rendererMenuTest.child("rendererRow2").opacity, 1);
        }

        /**
         * Tests that the menu drops down 8 px under the anchor, aligned to its right edge.
         */
        function test_open_downwards_under_the_anchor() {
            rendererMenuTest.openMenu();
            const panel = rendererMenuTest.child("rendererMenuPanel");
            const panelTopRight = panel.mapToItem(root, panel.width, 0);
            rendererMenuTest.compare(panelTopRight.x, anchor.x + anchor.width);
            rendererMenuTest.compare(panelTopRight.y, anchor.y + anchor.height + 8);
        }

        /**
         * Tests that the menu opens upwards 8 px above the anchor, aligned to its right edge.
         */
        function test_open_upwards_above_the_anchor() {
            anchor.y = 400;
            menu.opensUpwards = true;
            rendererMenuTest.openMenu();
            const panel = rendererMenuTest.child("rendererMenuPanel");
            const panelBottomRight = panel.mapToItem(root, panel.width, panel.height);
            rendererMenuTest.compare(panelBottomRight.x, anchor.x + anchor.width);
            rendererMenuTest.compare(panelBottomRight.y, anchor.y - 8);
        }

        /**
         * Tests that the menu is as high as its rows when they fit and doesn't scroll.
         */
        function test_not_scroll_when_the_rows_fit() {
            rendererMenuTest.openMenu();
            const list = rendererMenuTest.child("rendererList");
            rendererMenuTest.compare(list.height, 3 * 64);
            rendererMenuTest.compare(list.interactive, false);
        }

        /**
         * Tests that the height of the menu dropping down is capped to the space below the anchor and the list
         * scrolls inside.
         */
        function test_cap_the_height_downwards_and_scroll() {
            rendererMenuTest.fillUpTo(12);
            rendererMenuTest.openMenu();
            const panel = rendererMenuTest.child("rendererMenuPanel");
            const panelBottom = panel.mapToItem(root, 0, panel.height).y;
            rendererMenuTest.verify(panelBottom <= root.height - 8, panelBottom);
            const list = rendererMenuTest.child("rendererList");
            rendererMenuTest.verify(list.height < 12 * 64);
            rendererMenuTest.compare(list.interactive, true);
            rendererMenuTest.compare(list.clip, true);
            list.positionViewAtEnd();
            rendererMenuTest.compare(list.atYEnd, true);
            rendererMenuTest.verify(list.contentY > 0);
        }

        /**
         * Tests that a list scrolled the last time starts at its first row when the menu opens again.
         */
        function test_start_at_the_first_row_when_opened_again() {
            rendererMenuTest.fillUpTo(12);
            rendererMenuTest.openMenu();
            const list = rendererMenuTest.child("rendererList");
            list.positionViewAtEnd();
            rendererMenuTest.verify(list.contentY > 0);
            menu.close();
            rendererMenuTest.openMenu();
            rendererMenuTest.compare(list.contentY, 0);
        }

        /**
         * Tests that the height of the menu opening upwards is capped to the space above the anchor and the list
         * scrolls inside.
         */
        function test_cap_the_height_upwards_and_scroll() {
            rendererMenuTest.fillUpTo(12);
            anchor.y = 300;
            menu.opensUpwards = true;
            rendererMenuTest.openMenu();
            const panel = rendererMenuTest.child("rendererMenuPanel");
            const panelTop = panel.mapToItem(root, 0, 0).y;
            rendererMenuTest.verify(panelTop >= 8, panelTop);
            rendererMenuTest.compare(panel.mapToItem(root, 0, panel.height).y, anchor.y - 8);
            const list = rendererMenuTest.child("rendererList");
            rendererMenuTest.verify(list.height < 12 * 64);
            rendererMenuTest.compare(list.interactive, true);
        }
    }
}
