// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Objects
import Blabby.Shell

Item {
    id: root
    // The window width of 1280 px minus the navigation rail.
    width: 1200
    height: 1280

    ListModel {
        id: renderers
        ListElement {
            name: "Bathroom"
            playbackState: Renderer.NoMedia
            active: false
        }
        ListElement {
            name: "Kitchen"
            playbackState: Renderer.Playing
            active: true
        }
        ListElement {
            name: "Living Room"
            playbackState: Renderer.Stopped
            active: false
        }
    }

    ListModel {
        id: noRenderers
    }

    RenderersView {
        id: renderersView
        width: root.width
        height: 640
        model: renderers
    }

    // Placed below the view with Renderers to not overlap it.
    RenderersView {
        id: emptyView
        y: renderersView.height
        width: root.width
        height: renderersView.height
        model: noRenderers
    }

    TestCase {
        id: renderersViewTest
        name: "RenderersViewShould"
        when: windowShown

        SignalSpy {
            id: activatedSpy
            target: renderersView
            signalName: "activated"
        }

        SignalSpy {
            id: rescanRequestedSpy
            target: renderersView
            signalName: "rescanRequested"
        }

        function init() {
            renderersView.width = root.width;
            renderersView.scanning = false;
            emptyView.scanning = false;
            noRenderers.clear();
            activatedSpy.clear();
            rescanRequestedSpy.clear();
        }

        /**
         * Gives the child of the view with the objectName and fails the test when it doesn't exist.
         */
        function child(view, objectName) {
            const item = renderersViewTest.findChild(view, objectName);
            renderersViewTest.verify(item, objectName);
            return item;
        }

        /**
         * Gives the card of the Renderer at index and fails the test when it doesn't exist.
         */
        function card(index) {
            const rendererCard = renderersViewTest.findChild(renderersView, "rendererCard" + index);
            renderersViewTest.verify(rendererCard, "rendererCard" + index);
            return rendererCard;
        }

        /**
         * The widths of the view with the expected number of columns.
         */
        function test_reflow_the_cards_to_the_available_width_data() {
            // The widths are the window widths minus the navigation rail.
            return [
                {
                    tag: "800px window",
                    width: 720,
                    columns: 2
                },
                {
                    tag: "1280px window",
                    width: 1200,
                    columns: 3
                }
            ];
        }

        /**
         * Tests that the grid shows 2 columns in a 800 px and 3 columns in a 1280 px wide window.
         */
        function test_reflow_the_cards_to_the_available_width(data) {
            renderersView.width = data.width;
            renderersViewTest.waitForRendering(renderersView);
            const cardsInFirstRow = [0, 1, 2].filter(index => renderersViewTest.card(index).mapToItem(renderersView, 0, 0).y === renderersViewTest.card(0).mapToItem(renderersView, 0, 0).y);
            renderersViewTest.compare(cardsInFirstRow.length, data.columns);
        }

        /**
         * Tests that the card of the Active Renderer is highlighted.
         */
        function test_highlight_the_active_renderer() {
            renderersViewTest.compare(renderersViewTest.card(0).selected, false);
            renderersViewTest.compare(renderersViewTest.card(1).selected, true);
            renderersViewTest.compare(renderersViewTest.card(1).playbackState, Renderer.Playing);
        }

        /**
         * Tests that tapping a card emits activated with the index of the Renderer.
         */
        function test_emit_activated_with_the_index_of_the_tapped_card() {
            renderersViewTest.mouseClick(renderersViewTest.card(2));
            renderersViewTest.compare(activatedSpy.count, 1);
            renderersViewTest.compare(activatedSpy.signalArguments[0][0], 2);
        }

        /**
         * Tests that the rescan button emits rescanRequested and is busy while scanning.
         */
        function test_request_a_rescan_with_the_rescan_button() {
            const rescanButton = renderersViewTest.child(renderersView, "rescanButton");
            renderersViewTest.compare(rescanButton.variant, Button.Outlined);
            renderersViewTest.compare(rescanButton.busy, false);
            renderersViewTest.mouseClick(rescanButton);
            renderersViewTest.compare(rescanRequestedSpy.count, 1);

            renderersView.scanning = true;
            renderersViewTest.compare(rescanButton.busy, true);
            renderersViewTest.mouseClick(rescanButton);
            renderersViewTest.compare(rescanRequestedSpy.count, 1);
        }

        /**
         * Tests that no empty state is shown while there are Renderers.
         */
        function test_hide_the_empty_state_when_there_are_renderers() {
            renderersViewTest.compare(renderersViewTest.child(renderersView, "emptyState").visible, false);
        }

        /**
         * Tests that the empty state shows the searching variant while scanning.
         */
        function test_show_searching_while_scanning_without_renderers() {
            emptyView.scanning = true;
            const emptyState = renderersViewTest.child(emptyView, "emptyState");
            renderersViewTest.compare(emptyState.visible, true);
            renderersViewTest.compare(emptyState.busy, true);
            renderersViewTest.compare(emptyState.title, "Searching…");
            renderersViewTest.compare(emptyState.hint, "");
        }

        /**
         * Tests that the empty state shows the not found variant when no scan is running.
         */
        function test_show_not_found_without_renderers() {
            const emptyState = renderersViewTest.child(emptyView, "emptyState");
            renderersViewTest.compare(emptyState.visible, true);
            renderersViewTest.compare(emptyState.busy, false);
            renderersViewTest.compare(emptyState.title, "No renderers found");
            renderersViewTest.compare(emptyState.hint, "Make sure your speakers are switched on and on the same network");
        }

        /**
         * Tests that the empty state disappears as soon as a Renderer appears.
         */
        function test_hide_the_empty_state_when_a_renderer_appears() {
            emptyView.scanning = true;
            const emptyState = renderersViewTest.child(emptyView, "emptyState");
            renderersViewTest.compare(emptyState.visible, true);
            noRenderers.append({
                "name": "Kitchen",
                "playbackState": Renderer.NoMedia,
                "active": false
            });
            renderersViewTest.tryCompare(emptyState, "visible", false);
        }
    }
}
