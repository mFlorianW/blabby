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

    Card {
        id: card
        width: 300
        height: 96

        Rectangle {
            id: slotted
            width: 10
            height: 10
        }
    }

    TestCase {
        id: cardTest
        name: "CardShould"
        when: windowShown

        SignalSpy {
            id: clickedSpy
            target: card
            signalName: "clicked"
        }

        function init() {
            card.selected = false;
            card.enabled = true;
            clickedSpy.clear();
        }

        /**
         * Tests that items declared inside the card are placed in the content slot, inset by the padding.
         */
        function test_place_declared_items_in_the_content_slot() {
            const content = cardTest.findChild(card, "content");
            cardTest.verify(content);
            cardTest.compare(slotted.parent, content);
            const pos = slotted.mapToItem(card, 0, 0);
            cardTest.compare(pos.x, card.padding);
            cardTest.compare(pos.y, card.padding);
        }

        /**
         * Tests that a selected card is drawn in the secondary container colour with a primary outline.
         */
        function test_highlight_the_selected_card() {
            const container = cardTest.findChild(card, "container");
            cardTest.verify(Qt.colorEqual(container.color, Theme.colors.surfaceContainerLow));
            cardTest.verify(Qt.colorEqual(container.border.color, Theme.colors.outlineVariant));
            card.selected = true;
            cardTest.verify(Qt.colorEqual(container.color, Theme.colors.secondaryContainer));
            cardTest.verify(Qt.colorEqual(container.border.color, Theme.colors.primary));
        }

        /**
         * Tests that clicking an enabled card emits clicked.
         */
        function test_emit_clicked_when_clicked() {
            cardTest.mouseClick(card);
            cardTest.compare(clickedSpy.count, 1);
        }

        /**
         * Tests that a disabled card is dimmed and doesn't emit clicked.
         */
        function test_ignore_clicks_and_dim_when_disabled() {
            card.enabled = false;
            cardTest.mouseClick(card);
            cardTest.compare(clickedSpy.count, 0);
            cardTest.compare(card.opacity, Theme.disabledOpacity);
        }
    }
}
