// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Objects
import Blabby.Shell

Item {
    id: root
    width: 400
    height: 200

    RendererCard {
        id: rendererCard
        width: 300
        name: "Kitchen"
    }

    TestCase {
        id: rendererCardTest
        name: "RendererCardShould"
        when: windowShown

        function init() {
            rendererCard.playbackState = Renderer.NoMedia;
        }

        /**
         * Tests that the card shows the name of the Renderer.
         */
        function test_show_the_renderer_name() {
            rendererCardTest.compare(rendererCardTest.findChild(rendererCard, "name").text, "Kitchen");
        }

        /**
         * The shown text and emphasis for each Playback State except No Media.
         */
        function test_show_the_playback_state_data() {
            return [
                {
                    tag: "Playing",
                    state: Renderer.Playing,
                    text: "Playing",
                    emphasis: true
                },
                {
                    tag: "Paused",
                    state: Renderer.Paused,
                    text: "Paused",
                    emphasis: false
                },
                {
                    tag: "Stopped",
                    state: Renderer.Stopped,
                    text: "Stopped",
                    emphasis: false
                }
            ];
        }

        /**
         * Tests that the card shows the Playback State and emphasises Playing.
         */
        function test_show_the_playback_state(data) {
            rendererCard.playbackState = data.state;
            const playbackState = rendererCardTest.findChild(rendererCard, "playbackState");
            rendererCardTest.compare(playbackState.visible, true);
            rendererCardTest.compare(playbackState.text, data.text);
            rendererCardTest.compare(playbackState.emphasis, data.emphasis);
        }

        /**
         * Tests that the card shows no Playback State for No Media.
         */
        function test_show_nothing_for_no_media() {
            rendererCardTest.compare(rendererCardTest.findChild(rendererCard, "playbackState").visible, false);
        }
    }
}
