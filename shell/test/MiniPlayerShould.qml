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
    // The window width of 1280 px minus the navigation rail.
    width: 1200
    height: 120

    MiniPlayer {
        id: miniPlayer
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
    }

    TestCase {
        id: miniPlayerTest
        name: "MiniPlayerShould"
        when: windowShown

        SignalSpy {
            id: togglePlaybackRequestedSpy
            target: miniPlayer
            signalName: "togglePlaybackRequested"
        }

        SignalSpy {
            id: previousRequestedSpy
            target: miniPlayer
            signalName: "previousRequested"
        }

        SignalSpy {
            id: nextRequestedSpy
            target: miniPlayer
            signalName: "nextRequested"
        }

        SignalSpy {
            id: nowPlayingRequestedSpy
            target: miniPlayer
            signalName: "nowPlayingRequested"
        }

        function init() {
            miniPlayer.hasActiveRenderer = true;
            miniPlayer.rendererName = "Kitchen";
            miniPlayer.playbackState = Renderer.Playing;
            miniPlayer.trackTitle = "Harbour Lights";
            miniPlayer.trackArtist = "The Quiet Ferries";
            miniPlayer.artworkUrl = "";
            miniPlayer.canPause = true;
            miniPlayer.transitioning = false;
            miniPlayer.hasQueue = true;
            miniPlayer.hasPrevious = true;
            miniPlayer.hasNext = true;
            togglePlaybackRequestedSpy.clear();
            previousRequestedSpy.clear();
            nextRequestedSpy.clear();
            nowPlayingRequestedSpy.clear();
            miniPlayerTest.waitForLayout();
        }

        /**
         * Waits until the rows and columns of the mini player are laid out, so positions are final and clicks hit.
         */
        function waitForLayout() {
            miniPlayerTest.waitForItemPolished(miniPlayerTest.child("playPauseButton").parent);
            miniPlayerTest.waitForItemPolished(miniPlayerTest.child("trackTitle").parent);
        }

        /**
         * Gives the child of the mini player with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = miniPlayerTest.findChild(miniPlayer, objectName);
            miniPlayerTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the mini player is an 80 px high bar with rounded corners.
         */
        function test_be_an_80_px_rounded_bar() {
            miniPlayerTest.compare(miniPlayer.height, 80);
            miniPlayerTest.verify(miniPlayerTest.child("background").radius > 0);
        }

        /**
         * Tests that the title and the artist of the Current Track are shown.
         */
        function test_show_the_title_and_the_artist_of_the_current_track() {
            miniPlayerTest.compare(miniPlayerTest.child("trackTitle").text, "Harbour Lights");
            miniPlayerTest.compare(miniPlayerTest.child("trackArtist").text, "The Quiet Ferries");
            miniPlayerTest.compare(miniPlayerTest.child("trackArtist").visible, true);
        }

        /**
         * Tests that an unknown artist is hidden.
         */
        function test_hide_an_unknown_artist() {
            miniPlayer.trackArtist = "";
            miniPlayerTest.compare(miniPlayerTest.child("trackArtist").visible, false);
        }

        /**
         * Tests that the artwork of the Current Track is shown.
         */
        function test_show_the_artwork_of_the_current_track() {
            miniPlayer.artworkUrl = "qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg";
            const artwork = miniPlayerTest.child("trackArtwork");
            miniPlayerTest.compare(artwork.source, miniPlayer.artworkUrl);
            miniPlayerTest.tryCompare(artwork, "hasArtwork", true);
        }

        /**
         * Tests that a placeholder is shown without artwork.
         */
        function test_show_a_placeholder_without_artwork() {
            const artwork = miniPlayerTest.child("trackArtwork");
            miniPlayerTest.compare(artwork.visible, true);
            miniPlayerTest.compare(artwork.hasArtwork, false);
        }

        /**
         * Tests that a long title and artist are elided without pushing the controls off.
         */
        function test_elide_a_long_title_and_artist_without_pushing_the_controls_off() {
            miniPlayer.trackTitle = "A very long title ".repeat(20);
            miniPlayer.trackArtist = "A very long artist ".repeat(20);
            miniPlayerTest.waitForLayout();
            const title = miniPlayerTest.child("trackTitle");
            const artist = miniPlayerTest.child("trackArtist");
            miniPlayerTest.compare(title.truncated, true);
            miniPlayerTest.compare(artist.truncated, true);
            const previousX = miniPlayerTest.child("previousButton").mapToItem(miniPlayer, 0, 0).x;
            miniPlayerTest.verify(title.mapToItem(miniPlayer, title.width, 0).x <= previousX);
            miniPlayerTest.verify(artist.mapToItem(miniPlayer, artist.width, 0).x <= previousX);
        }

        /**
         * Tests that a long title ends before Play/Pause while previous and next are hidden.
         */
        function test_elide_a_long_title_before_play_pause_without_a_queue() {
            miniPlayer.hasQueue = false;
            miniPlayer.trackTitle = "A very long title ".repeat(20);
            miniPlayerTest.waitForLayout();
            const title = miniPlayerTest.child("trackTitle");
            miniPlayerTest.compare(title.truncated, true);
            const playPauseX = miniPlayerTest.child("playPauseButton").mapToItem(miniPlayer, 0, 0).x;
            miniPlayerTest.verify(title.mapToItem(miniPlayer, title.width, 0).x <= playPauseX);
        }

        /**
         * Tests that the name of the Active Renderer is shown with a speaker icon in a pill on the right.
         */
        function test_show_the_active_renderer_in_a_pill_on_the_right() {
            const pill = miniPlayerTest.child("rendererPill");
            miniPlayerTest.compare(pill.text, "Kitchen");
            miniPlayerTest.compare(pill.iconSource, "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg");
            miniPlayerTest.verify(pill.mapToItem(miniPlayer, 0, 0).x > miniPlayerTest.child("nextButton").mapToItem(miniPlayer, 0, 0).x);
        }

        /**
         * Tests that the Renderer pill doesn't react to taps yet.
         */
        function test_not_react_to_taps_on_the_renderer_pill() {
            miniPlayerTest.compare(miniPlayerTest.child("rendererPill").enabled, false);
        }

        /**
         * Tests that "No Renderer" and a "Choose a Renderer" pill are shown and the controls are disabled without an
         * Active Renderer.
         */
        function test_show_no_renderer_without_an_active_renderer() {
            miniPlayer.hasActiveRenderer = false;
            miniPlayer.rendererName = "";
            miniPlayer.playbackState = Renderer.NoMedia;
            miniPlayerTest.compare(miniPlayerTest.child("trackTitle").text, "No Renderer");
            miniPlayerTest.compare(miniPlayerTest.child("trackArtist").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("rendererPill").text, "Choose a Renderer");
            miniPlayerTest.compare(miniPlayerTest.child("playPauseButton").enabled, false);
            miniPlayerTest.compare(miniPlayerTest.child("previousButton").enabled, false);
            miniPlayerTest.compare(miniPlayerTest.child("nextButton").enabled, false);
        }

        /**
         * Tests that "Nothing playing" is shown and the controls are disabled while the Active Renderer has no media.
         */
        function test_show_nothing_playing_with_disabled_controls_without_media() {
            miniPlayer.playbackState = Renderer.NoMedia;
            miniPlayerTest.compare(miniPlayerTest.child("trackTitle").text, "Nothing playing");
            miniPlayerTest.compare(miniPlayerTest.child("trackArtist").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("trackArtwork").hasArtwork, false);
            miniPlayerTest.compare(miniPlayerTest.child("rendererPill").text, "Kitchen");
            const playPauseButton = miniPlayerTest.child("playPauseButton");
            miniPlayerTest.compare(playPauseButton.enabled, false);
            miniPlayerTest.compare(miniPlayerTest.child("previousButton").enabled, false);
            miniPlayerTest.compare(miniPlayerTest.child("nextButton").enabled, false);
            miniPlayerTest.mouseClick(playPauseButton);
            miniPlayerTest.compare(togglePlaybackRequestedSpy.count, 0);
        }

        function test_open_the_playing_screen_with_a_tap_on_the_track_area_data() {
            return [
                {
                    tag: "Playing",
                    hasActiveRenderer: true,
                    playbackState: Renderer.Playing,
                    target: "trackTitle"
                },
                {
                    tag: "artwork",
                    hasActiveRenderer: true,
                    playbackState: Renderer.Paused,
                    target: "trackArtwork"
                },
                {
                    tag: "No Media",
                    hasActiveRenderer: true,
                    playbackState: Renderer.NoMedia,
                    target: "trackTitle"
                },
                {
                    tag: "No Renderer",
                    hasActiveRenderer: false,
                    playbackState: Renderer.NoMedia,
                    target: "trackTitle"
                }
            ];
        }

        /**
         * Tests that a tap on the artwork or the title asks to open the Playing screen in every state.
         */
        function test_open_the_playing_screen_with_a_tap_on_the_track_area(data) {
            miniPlayer.hasActiveRenderer = data.hasActiveRenderer;
            miniPlayer.playbackState = data.playbackState;
            miniPlayerTest.waitForLayout();
            miniPlayerTest.mouseClick(miniPlayerTest.child(data.target));
            miniPlayerTest.compare(nowPlayingRequestedSpy.count, 1);
        }

        /**
         * Tests that the pause icon is shown while playing.
         */
        function test_show_the_pause_icon_while_playing() {
            const icon = miniPlayerTest.findChild(miniPlayerTest.child("playPauseButton"), "icon");
            miniPlayerTest.compare(icon.source, "qrc:/qt/qml/Blabby/Shell/icons/material/pause.svg");
        }

        /**
         * Tests that the stop icon is shown while playing on a Renderer that can't pause.
         */
        function test_show_the_stop_icon_while_playing_on_a_renderer_that_cannot_pause() {
            miniPlayer.canPause = false;
            const icon = miniPlayerTest.findChild(miniPlayerTest.child("playPauseButton"), "icon");
            miniPlayerTest.compare(icon.source, "qrc:/qt/qml/Blabby/Shell/icons/material/stop.svg");
        }

        function test_show_the_play_icon_while_not_playing_data() {
            return [
                {
                    tag: "Paused",
                    playbackState: Renderer.Paused
                },
                {
                    tag: "Stopped",
                    playbackState: Renderer.Stopped
                }
            ];
        }

        /**
         * Tests that the play icon is shown while the Active Renderer doesn't play.
         */
        function test_show_the_play_icon_while_not_playing(data) {
            miniPlayer.playbackState = data.playbackState;
            const icon = miniPlayerTest.findChild(miniPlayerTest.child("playPauseButton"), "icon");
            miniPlayerTest.compare(icon.source, "qrc:/qt/qml/Blabby/Shell/icons/material/play_arrow.svg");
        }

        /**
         * Tests that the Play/Pause button shows a busy ring while the Active Renderer is transitioning.
         */
        function test_show_a_busy_ring_while_transitioning() {
            const busyRing = miniPlayerTest.findChild(miniPlayerTest.child("playPauseButton"), "busyRing");
            miniPlayerTest.compare(busyRing.running, false);
            miniPlayer.transitioning = true;
            miniPlayerTest.compare(busyRing.running, true);
        }

        /**
         * Tests that the Play/Pause button asks to toggle the playback.
         */
        function test_ask_to_toggle_the_playback_with_the_play_pause_button() {
            const button = miniPlayerTest.child("playPauseButton");
            miniPlayerTest.verify(button.width >= 44 && button.height >= 44);
            miniPlayerTest.mouseClick(button);
            miniPlayerTest.compare(togglePlaybackRequestedSpy.count, 1);
        }

        /**
         * Tests that previous and next are shown beside Play/Pause while the Queue has entries.
         */
        function test_show_previous_and_next_beside_play_pause_while_the_queue_has_entries() {
            const previousButton = miniPlayerTest.child("previousButton");
            const playPauseButton = miniPlayerTest.child("playPauseButton");
            const nextButton = miniPlayerTest.child("nextButton");
            miniPlayerTest.compare(previousButton.visible, true);
            miniPlayerTest.compare(nextButton.visible, true);
            miniPlayerTest.verify(previousButton.mapToItem(miniPlayer, 0, 0).x < playPauseButton.mapToItem(miniPlayer, 0, 0).x);
            miniPlayerTest.verify(nextButton.mapToItem(miniPlayer, 0, 0).x > playPauseButton.mapToItem(miniPlayer, 0, 0).x);
        }

        /**
         * Tests that previous and next are hidden while the Queue is empty.
         */
        function test_hide_previous_and_next_while_the_queue_is_empty() {
            miniPlayer.hasQueue = false;
            miniPlayerTest.compare(miniPlayerTest.child("previousButton").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("nextButton").visible, false);
        }

        function test_ask_for_previous_and_next_with_their_buttons_data() {
            return [
                {
                    tag: "previous",
                    button: "previousButton",
                    spy: previousRequestedSpy
                },
                {
                    tag: "next",
                    button: "nextButton",
                    spy: nextRequestedSpy
                }
            ];
        }

        /**
         * Tests that the previous and the next button ask for previous and next.
         */
        function test_ask_for_previous_and_next_with_their_buttons(data) {
            miniPlayerTest.mouseClick(miniPlayerTest.child(data.button));
            miniPlayerTest.compare(data.spy.count, 1);
        }

        function test_disable_previous_and_next_when_unavailable_data() {
            return [
                {
                    tag: "previous",
                    property: "hasPrevious",
                    button: "previousButton",
                    spy: previousRequestedSpy
                },
                {
                    tag: "next",
                    property: "hasNext",
                    button: "nextButton",
                    spy: nextRequestedSpy
                }
            ];
        }

        /**
         * Tests that previous and next are disabled at the ends of the Queue.
         */
        function test_disable_previous_and_next_when_unavailable(data) {
            miniPlayer[data.property] = false;
            const button = miniPlayerTest.child(data.button);
            miniPlayerTest.compare(button.enabled, false);
            miniPlayerTest.compare(button.opacity, Theme.disabledOpacity);
            miniPlayerTest.mouseClick(button);
            miniPlayerTest.compare(data.spy.count, 0);
        }
    }
}
