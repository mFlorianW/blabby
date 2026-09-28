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
            rendererCard.manufacturer = "";
            rendererCard.modelName = "";
            rendererCard.address = "";
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
         * The shown details for each combination of present and missing fields.
         */
        function test_show_the_details_data() {
            return [
                {
                    tag: "all",
                    manufacturer: "Denon",
                    modelName: "HEOS 1",
                    address: "192.168.1.42",
                    text: "Denon · HEOS 1 · 192.168.1.42"
                },
                {
                    tag: "no manufacturer",
                    manufacturer: "",
                    modelName: "HEOS 1",
                    address: "192.168.1.42",
                    text: "HEOS 1 · 192.168.1.42"
                },
                {
                    tag: "no model",
                    manufacturer: "Denon",
                    modelName: "",
                    address: "192.168.1.42",
                    text: "Denon · 192.168.1.42"
                },
                {
                    tag: "no address",
                    manufacturer: "Denon",
                    modelName: "HEOS 1",
                    address: "",
                    text: "Denon · HEOS 1"
                },
                {
                    tag: "blank model",
                    manufacturer: "Denon",
                    modelName: "  ",
                    address: "192.168.1.42",
                    text: "Denon · 192.168.1.42"
                },
                {
                    tag: "only address",
                    manufacturer: "",
                    modelName: "",
                    address: "192.168.1.42",
                    text: "192.168.1.42"
                }
            ];
        }

        /**
         * Tests that the card shows manufacturer, model and address separated by dots and omits missing fields.
         */
        function test_show_the_details(data) {
            rendererCard.manufacturer = data.manufacturer;
            rendererCard.modelName = data.modelName;
            rendererCard.address = data.address;
            const details = rendererCardTest.findChild(rendererCard, "details");
            rendererCardTest.compare(details.visible, true);
            rendererCardTest.compare(details.text, data.text);
        }

        /**
         * Tests that the card shows no details line when all fields are missing.
         */
        function test_show_no_details_when_all_fields_are_missing() {
            rendererCardTest.compare(rendererCardTest.findChild(rendererCard, "details").visible, false);
        }

        /**
         * Tests that too long details are elided instead of overflowing the card.
         */
        function test_elide_too_long_details() {
            rendererCard.manufacturer = "A Manufacturer With A Really Very Long Name";
            rendererCard.modelName = "A Model With An Even Longer Name Than The Manufacturer";
            rendererCard.address = "192.168.1.42";
            const details = rendererCardTest.findChild(rendererCard, "details");
            rendererCardTest.compare(details.elide, Text.ElideRight);
            rendererCardTest.verify(details.truncated);
            rendererCardTest.verify(details.width <= rendererCard.width);
        }

        /**
         * Tests that the card shows no Playback State for No Media.
         */
        function test_show_nothing_for_no_media() {
            rendererCardTest.compare(rendererCardTest.findChild(rendererCard, "playbackState").visible, false);
        }
    }
}
