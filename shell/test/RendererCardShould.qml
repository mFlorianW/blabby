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

        SignalSpy {
            id: clickedSpy
            target: rendererCard
            signalName: "clicked"
        }

        SignalSpy {
            id: forgetRequestedSpy
            target: rendererCard
            signalName: "forgetRequested"
        }

        /**
         * Gives the child of the card with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = rendererCardTest.findChild(rendererCard, objectName);
            rendererCardTest.verify(item, objectName);
            return item;
        }

        function init() {
            clickedSpy.clear();
            forgetRequestedSpy.clear();
            rendererCard.availability = Renderer.Online;
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
         * Tests that an Online card is enabled, not dimmed and emits clicked when tapped.
         */
        function test_be_enabled_when_online() {
            const card = rendererCardTest.child("card");
            rendererCardTest.compare(card.enabled, true);
            rendererCardTest.compare(card.opacity, 1);
            rendererCardTest.mouseClick(card);
            rendererCardTest.compare(clickedSpy.count, 1);
        }

        /**
         * Tests that only an Offline card offers to forget the Renderer.
         */
        function test_offer_forget_only_when_offline() {
            rendererCardTest.compare(rendererCardTest.child("forgetButton").visible, false);
            rendererCard.availability = Renderer.Offline;
            rendererCardTest.compare(rendererCardTest.child("forgetButton").visible, true);
        }

        /**
         * Tests that the forget button of an Offline card isn't dimmed and emits forgetRequested but not clicked.
         */
        function test_request_to_forget_with_the_forget_button() {
            rendererCard.availability = Renderer.Offline;
            const forgetButton = rendererCardTest.child("forgetButton");
            rendererCardTest.compare(forgetButton.enabled, true);
            rendererCardTest.compare(forgetButton.opacity, 1);
            rendererCardTest.compare(forgetButton.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/delete.svg"));
            rendererCardTest.mouseClick(forgetButton);
            rendererCardTest.compare(forgetRequestedSpy.count, 1);
            rendererCardTest.compare(clickedSpy.count, 0);
        }

        /**
         * Tests that the details of an Offline card leave room for the forget button.
         */
        function test_not_overlap_the_forget_button_with_the_details() {
            rendererCard.availability = Renderer.Offline;
            rendererCard.manufacturer = "A Manufacturer With A Really Very Long Name";
            const details = rendererCardTest.child("details");
            const forgetButton = rendererCardTest.child("forgetButton");
            const detailsRight = details.mapToItem(rendererCard, details.width, 0).x;
            rendererCardTest.verify(detailsRight <= forgetButton.x);
        }

        /**
         * Tests that an Offline card shows Offline instead of the Playback State, is dimmed and disabled.
         */
        function test_show_offline_and_be_disabled_when_offline() {
            rendererCard.playbackState = Renderer.Playing;
            rendererCard.availability = Renderer.Offline;
            const playbackState = rendererCardTest.findChild(rendererCard, "playbackState");
            rendererCardTest.compare(playbackState.visible, true);
            rendererCardTest.compare(playbackState.text, "Offline");
            rendererCardTest.compare(playbackState.emphasis, false);
            rendererCardTest.compare(playbackState.iconSource, "");
            const card = rendererCardTest.child("card");
            rendererCardTest.compare(card.enabled, false);
            rendererCardTest.compare(card.opacity, Theme.disabledOpacity);
            rendererCardTest.mouseClick(card);
            rendererCardTest.compare(clickedSpy.count, 0);
        }

        /**
         * Tests that an Offline card keeps showing the last known details.
         */
        function test_show_the_last_known_details_when_offline() {
            rendererCard.availability = Renderer.Offline;
            rendererCard.address = "192.168.1.42";
            const details = rendererCardTest.findChild(rendererCard, "details");
            rendererCardTest.compare(details.visible, true);
            rendererCardTest.compare(details.text, "192.168.1.42");
        }

        /**
         * Tests that the card shows no Playback State for No Media.
         */
        function test_show_nothing_for_no_media() {
            rendererCardTest.compare(rendererCardTest.findChild(rendererCard, "playbackState").visible, false);
        }
    }
}
