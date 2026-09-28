// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Objects
import Blabby.Shell

Item {
    id: root
    // The window width of 1280 px minus the navigation rail.
    width: 1200
    height: 640

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

    RenderersView {
        id: renderersView
        width: root.width
        height: root.height
        model: renderers
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

        function init() {
            renderersView.width = root.width;
            activatedSpy.clear();
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
    }
}
