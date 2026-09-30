// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 100
    height: 100

    EqualizerGlyph {
        id: glyph
        width: 24
        height: 24
    }

    TestCase {
        id: equalizerGlyphTest
        name: "EqualizerGlyphShould"
        when: windowShown

        function init() {
            glyph.running = false;
            equalizerGlyphTest.wait(50);
        }

        /**
         * Gives the heights of the bars from left to right.
         */
        function barHeights(): list<real> {
            return [0, 1, 2, 3, 4].map(bar => equalizerGlyphTest.findChild(glyph, "bar" + bar).height);
        }

        /**
         * Tests that a glyph that doesn't run keeps the bars of the equaliser icon.
         */
        function test_keep_its_bars_while_not_running() {
            const heights = equalizerGlyphTest.barHeights();
            equalizerGlyphTest.wait(300);
            equalizerGlyphTest.compare(equalizerGlyphTest.barHeights(), heights);
            equalizerGlyphTest.compare(heights, [4, 12, 20, 12, 4]);
        }

        /**
         * Tests that a running glyph moves its bars and returns to the icon when it stops.
         */
        function test_move_its_bars_while_running() {
            const heights = equalizerGlyphTest.barHeights();
            glyph.running = true;
            equalizerGlyphTest.tryVerify(() => JSON.stringify(equalizerGlyphTest.barHeights()) !== JSON.stringify(heights));
            glyph.running = false;
            equalizerGlyphTest.tryCompare(equalizerGlyphTest.findChild(glyph, "bar2"), "height", 20);
        }
    }
}
