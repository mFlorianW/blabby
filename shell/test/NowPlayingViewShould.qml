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
    height: 640

    NowPlayingView {
        id: nowPlayingView
        anchors.fill: parent
    }

    TestCase {
        id: nowPlayingViewTest
        name: "NowPlayingViewShould"
        when: windowShown

        SignalSpy {
            id: chooseRendererRequestedSpy
            target: nowPlayingView
            signalName: "chooseRendererRequested"
        }

        function init() {
            nowPlayingView.hasActiveRenderer = true;
            nowPlayingView.rendererName = "Kitchen";
            nowPlayingView.playbackState = Renderer.NoMedia;
            nowPlayingView.trackTitle = "Harbour Lights";
            nowPlayingView.trackArtist = "The Quiet Ferries";
            nowPlayingView.artworkUrl = "";
            nowPlayingView.trackAlbum = "Low Tide Sessions";
            nowPlayingView.trackYear = "2024";
            nowPlayingView.trackFormat = "FLAC · 24-bit / 96 kHz";
            nowPlayingViewTest.child("toast").hide();
            chooseRendererRequestedSpy.clear();
            // The actions of the header are laid out on the next polish, clicks before would miss them.
            nowPlayingViewTest.waitForItemPolished(nowPlayingViewTest.child("rendererPill").parent);
        }

        /**
         * Gives the child of the view with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = nowPlayingViewTest.findChild(nowPlayingView, objectName);
            nowPlayingViewTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that "Choose a Renderer" with a button is shown without an Active Renderer.
         */
        function test_show_choose_a_renderer_without_an_active_renderer() {
            nowPlayingView.hasActiveRenderer = false;
            nowPlayingView.rendererName = "";
            const emptyState = nowPlayingViewTest.child("emptyState");
            nowPlayingViewTest.compare(emptyState.visible, true);
            nowPlayingViewTest.compare(emptyState.title, "Choose a Renderer");
            nowPlayingViewTest.compare(emptyState.actionText, "Choose Renderer");
            nowPlayingViewTest.compare(nowPlayingViewTest.child("rendererPill").visible, false);
        }

        /**
         * Tests that the button of the "Choose a Renderer" empty state asks to choose a Renderer.
         */
        function test_ask_to_choose_a_renderer_with_the_choose_a_renderer_button() {
            nowPlayingView.hasActiveRenderer = false;
            nowPlayingView.rendererName = "";
            const action = nowPlayingViewTest.findChild(nowPlayingViewTest.child("emptyState"), "action");
            nowPlayingViewTest.verify(action);
            nowPlayingViewTest.waitForItemPolished(action.parent);
            nowPlayingViewTest.mouseClick(action);
            nowPlayingViewTest.compare(chooseRendererRequestedSpy.count, 1);
        }

        /**
         * Tests that "Nothing playing on ‹Renderer›" is shown for an Active Renderer in No Media.
         */
        function test_show_nothing_playing_for_an_active_renderer_without_media() {
            const emptyState = nowPlayingViewTest.child("emptyState");
            nowPlayingViewTest.compare(emptyState.visible, true);
            nowPlayingViewTest.compare(emptyState.title, "Nothing playing on Kitchen");
            nowPlayingViewTest.compare(emptyState.actionText, "");
        }

        /**
         * Tests that no empty state is shown while the Active Renderer has media.
         */
        function test_show_no_empty_state_while_the_active_renderer_has_media() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("emptyState").visible, false);
        }

        /**
         * Tests that the Renderer pill shows the name of the Active Renderer and asks to choose a Renderer.
         */
        function test_ask_to_choose_a_renderer_with_the_renderer_pill() {
            const pill = nowPlayingViewTest.child("rendererPill");
            nowPlayingViewTest.compare(pill.visible, true);
            nowPlayingViewTest.compare(pill.text, "Kitchen");
            nowPlayingViewTest.mouseClick(pill);
            nowPlayingViewTest.compare(chooseRendererRequestedSpy.count, 1);
        }

        /**
         * Tests that the title and the artist of the Current Track are shown while the Active Renderer has media.
         */
        function test_show_the_title_and_the_artist_of_the_current_track() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("track").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("trackTitle").text, "Harbour Lights");
            const artist = nowPlayingViewTest.child("trackArtist");
            nowPlayingViewTest.compare(artist.visible, true);
            nowPlayingViewTest.compare(artist.text, "The Quiet Ferries");
        }

        /**
         * Tests that the artist is hidden when it is unknown.
         */
        function test_hide_an_unknown_artist() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.trackArtist = "";
            nowPlayingViewTest.compare(nowPlayingViewTest.child("trackArtist").visible, false);
        }

        /**
         * Tests that a Stopped Renderer shows its Current Track.
         */
        function test_show_the_current_track_of_a_stopped_renderer() {
            nowPlayingView.playbackState = Renderer.Stopped;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("track").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("emptyState").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("trackTitle").text, "Harbour Lights");
        }

        /**
         * Tests that no Current Track is shown while the Active Renderer has no media.
         */
        function test_show_no_current_track_without_media() {
            nowPlayingViewTest.compare(nowPlayingViewTest.child("track").visible, false);
        }

        /**
         * Tests that the artwork of the Current Track is shown instead of the placeholder glyph.
         */
        function test_show_the_artwork_of_the_current_track() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.artworkUrl = "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg";
            const artwork = nowPlayingViewTest.child("trackArtwork");
            nowPlayingViewTest.tryCompare(nowPlayingViewTest.findChild(artwork, "artwork"), "visible", true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(artwork, "glyph").visible, false);
            nowPlayingViewTest.compare(artwork.width, artwork.height);
        }

        /**
         * Tests that a tinted placeholder with a note glyph is shown when the Current Track has no artwork.
         */
        function test_show_a_placeholder_without_artwork() {
            nowPlayingView.playbackState = Renderer.Playing;
            const artwork = nowPlayingViewTest.child("trackArtwork");
            const glyph = nowPlayingViewTest.findChild(artwork, "glyph");
            nowPlayingViewTest.compare(glyph.visible, true);
            nowPlayingViewTest.compare(glyph.source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"));
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(artwork, "artwork").visible, false);
        }

        function test_show_the_known_parts_of_album_and_year_data() {
            return [
                {
                    tag: "album and year",
                    album: "Low Tide Sessions",
                    year: "2024",
                    expected: "Low Tide Sessions · 2024"
                },
                {
                    tag: "album only",
                    album: "Low Tide Sessions",
                    year: "",
                    expected: "Low Tide Sessions"
                },
                {
                    tag: "year only",
                    album: "",
                    year: "2024",
                    expected: "2024"
                },
                {
                    tag: "neither album nor year",
                    album: "",
                    year: "",
                    expected: ""
                }
            ];
        }

        /**
         * Tests that "album · year" shows only the known parts and is hidden when both are unknown.
         */
        function test_show_the_known_parts_of_album_and_year(data) {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.trackAlbum = data.album;
            nowPlayingView.trackYear = data.year;
            const albumAndYear = nowPlayingViewTest.child("trackAlbumAndYear");
            nowPlayingViewTest.compare(albumAndYear.text, data.expected);
            nowPlayingViewTest.compare(albumAndYear.visible, data.expected !== "");
        }

        /**
         * Tests that the format chip shows the format of the Current Track.
         */
        function test_show_the_format_chip() {
            nowPlayingView.playbackState = Renderer.Playing;
            const chip = nowPlayingViewTest.child("formatChip");
            nowPlayingViewTest.compare(chip.visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(chip, "formatText").text, "FLAC · 24-bit / 96 kHz");
        }

        /**
         * Tests that the format chip is hidden without a format.
         */
        function test_hide_the_format_chip_without_a_format() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.trackFormat = "";
            nowPlayingViewTest.compare(nowPlayingViewTest.child("formatChip").visible, false);
        }

        /**
         * Tests that a toast tells that the Active Renderer went Offline.
         */
        function test_show_a_toast_when_the_active_renderer_went_offline() {
            const toast = nowPlayingViewTest.child("toast");
            nowPlayingViewTest.compare(toast.visible, false);
            nowPlayingView.hasActiveRenderer = false;
            nowPlayingView.rendererName = "";
            nowPlayingView.showActiveRendererWentOffline("Kitchen");
            nowPlayingViewTest.tryCompare(toast, "visible", true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(toast, "message").text, "Kitchen is no longer available");
            nowPlayingViewTest.compare(nowPlayingViewTest.child("emptyState").title, "Choose a Renderer");
        }
    }
}
