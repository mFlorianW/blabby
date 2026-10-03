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
    // High enough for the Renderer menu to open upwards above the mini player.
    height: 480

    ListModel {
        id: rendererModel
        ListElement {
            name: "Kitchen"
            availability: Renderer.Online
            active: true
        }
        ListElement {
            name: "Living Room"
            availability: Renderer.Online
            active: false
        }
    }

    MouseArea {
        id: beneath
        anchors.fill: root
    }

    MiniPlayer {
        id: miniPlayer
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        renderers: rendererModel
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

        SignalSpy {
            id: rendererPickedSpy
            target: miniPlayer
            signalName: "rendererPicked"
        }

        SignalSpy {
            id: volumeRequestedSpy
            target: miniPlayer
            signalName: "volumeRequested"
        }

        SignalSpy {
            id: muteRequestedSpy
            target: miniPlayer
            signalName: "muteRequested"
        }

        SignalSpy {
            id: beneathClickedSpy
            target: beneath
            signalName: "clicked"
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
            miniPlayer.volume = 42;
            miniPlayer.volumeMinimum = 0;
            miniPlayer.volumeMaximum = 60;
            miniPlayer.canControlVolume = true;
            miniPlayer.muted = false;
            miniPlayer.canControlMute = true;
            togglePlaybackRequestedSpy.clear();
            previousRequestedSpy.clear();
            nextRequestedSpy.clear();
            nowPlayingRequestedSpy.clear();
            rendererPickedSpy.clear();
            volumeRequestedSpy.clear();
            muteRequestedSpy.clear();
            beneathClickedSpy.clear();
            miniPlayer.visible = true;
            miniPlayerTest.child("rendererMenu").close();
            miniPlayerTest.waitForLayout();
        }

        /**
         * Waits until the rows and columns of the mini player are laid out, so positions are final and clicks hit.
         */
        function waitForLayout() {
            miniPlayerTest.waitForItemPolished(miniPlayerTest.child("playPauseButton").parent);
            miniPlayerTest.waitForItemPolished(miniPlayerTest.child("trackTitle").parent);
            miniPlayerTest.waitForItemPolished(miniPlayerTest.child("volumeControls"));
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
         * Opens the Renderer menu with the Renderer pill and gives it.
         */
        function openRendererMenu() {
            miniPlayerTest.mouseClick(miniPlayerTest.child("rendererPill"));
            const menu = miniPlayerTest.child("rendererMenu");
            miniPlayerTest.compare(menu.opened, true);
            // The rows are created on the polish after the list became visible, clicks before would miss them.
            miniPlayerTest.waitForItemPolished(miniPlayerTest.findChild(menu, "rendererList"));
            return menu;
        }

        /**
         * The variants with and without an Active Renderer.
         */
        function activeRendererVariants() {
            return [
                {
                    "tag": "with an Active Renderer",
                    "hasActiveRenderer": true
                },
                {
                    "tag": "without an Active Renderer",
                    "hasActiveRenderer": false
                }
            ];
        }

        function test_open_the_renderer_menu_upwards_with_the_renderer_pill_data() {
            return miniPlayerTest.activeRendererVariants();
        }

        /**
         * Tests that the Renderer pill opens the Renderer menu upwards 8 px above it, aligned to its right edge,
         * listing the Renderers and looking checked meanwhile, with and without an Active Renderer.
         */
        function test_open_the_renderer_menu_upwards_with_the_renderer_pill(data) {
            miniPlayer.hasActiveRenderer = data.hasActiveRenderer;
            const pill = miniPlayerTest.child("rendererPill");
            miniPlayerTest.compare(pill.enabled, true);
            const menu = miniPlayerTest.openRendererMenu();
            miniPlayerTest.compare(menu.opensUpwards, true);
            miniPlayerTest.compare(pill.checked, true);
            const panel = miniPlayerTest.findChild(menu, "rendererMenuPanel");
            miniPlayerTest.compare(panel.visible, true);
            const pillTopRight = pill.mapToItem(root, pill.width, 0);
            const panelBottomRight = panel.mapToItem(root, panel.width, panel.height);
            miniPlayerTest.compare(panelBottomRight.x, pillTopRight.x);
            miniPlayerTest.compare(panelBottomRight.y, pillTopRight.y - 8);
            miniPlayerTest.compare(menu.count, 2);
            miniPlayerTest.compare(rendererPickedSpy.count, 0);
        }

        function test_emit_rendererPicked_when_a_renderer_is_picked_data() {
            return miniPlayerTest.activeRendererVariants();
        }

        /**
         * Tests that picking a Renderer in the Renderer menu emits rendererPicked with its index and closes the menu,
         * with and without an Active Renderer.
         */
        function test_emit_rendererPicked_when_a_renderer_is_picked(data) {
            miniPlayer.hasActiveRenderer = data.hasActiveRenderer;
            const menu = miniPlayerTest.openRendererMenu();
            const row = miniPlayerTest.findChild(menu, "rendererRow1");
            miniPlayerTest.verify(row);
            miniPlayerTest.mouseClick(row);
            miniPlayerTest.compare(rendererPickedSpy.count, 1);
            miniPlayerTest.compare(rendererPickedSpy.signalArguments[0][0], 1);
            miniPlayerTest.compare(menu.opened, false);
            miniPlayerTest.compare(miniPlayerTest.child("rendererPill").checked, false);
            miniPlayerTest.compare(nowPlayingRequestedSpy.count, 0);
        }

        /**
         * Tests that a tap elsewhere on the screen closes the Renderer menu without reaching anything beneath, and that
         * the mini player reacts to taps again afterwards.
         */
        function test_close_the_renderer_menu_with_a_tap_elsewhere() {
            const menu = miniPlayerTest.openRendererMenu();
            miniPlayerTest.mouseClick(root, 50, 50);
            miniPlayerTest.compare(menu.opened, false);
            miniPlayerTest.compare(beneathClickedSpy.count, 0);
            miniPlayerTest.mouseClick(miniPlayerTest.child("trackArea"));
            miniPlayerTest.compare(nowPlayingRequestedSpy.count, 1);
        }

        /**
         * Tests that the Renderer menu is closed when the mini player becomes invisible, e.g. on the Playing screen.
         */
        function test_close_the_renderer_menu_when_the_mini_player_becomes_invisible() {
            const menu = miniPlayerTest.openRendererMenu();
            miniPlayer.visible = false;
            miniPlayerTest.compare(menu.opened, false);
            miniPlayer.visible = true;
            miniPlayerTest.compare(miniPlayerTest.findChild(menu, "rendererMenuPanel").visible, false);
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

        /**
         * Tests that a Mute button and a Volume slider of about 160 px without a number sit between the transport and
         * the Renderer pill, the slider showing the Volume in the range of the Renderer.
         */
        function test_show_mute_and_the_volume_between_the_transport_and_the_renderer_pill() {
            const muteButton = miniPlayerTest.child("muteButton");
            const slider = miniPlayerTest.child("volumeSlider");
            miniPlayerTest.compare(muteButton.visible, true);
            miniPlayerTest.compare(slider.visible, true);
            miniPlayerTest.compare(slider.width, 160);
            miniPlayerTest.compare(slider.from, 0);
            miniPlayerTest.compare(slider.to, 60);
            miniPlayerTest.compare(slider.value, 42);
            miniPlayerTest.compare(slider.valueIndicatorEnabled, false);
            miniPlayerTest.compare(miniPlayerTest.findChild(miniPlayer, "volumeValue"), null);
            const nextButton = miniPlayerTest.child("nextButton");
            const nextRight = nextButton.mapToItem(miniPlayer, nextButton.width, 0).x;
            const muteLeft = muteButton.mapToItem(miniPlayer, 0, 0).x;
            const sliderLeft = slider.mapToItem(miniPlayer, 0, 0).x;
            const sliderRight = slider.mapToItem(miniPlayer, slider.width, 0).x;
            const pillLeft = miniPlayerTest.child("rendererPill").mapToItem(miniPlayer, 0, 0).x;
            miniPlayerTest.verify(nextRight < muteLeft, `${nextRight} < ${muteLeft}`);
            miniPlayerTest.verify(muteLeft < sliderLeft, `${muteLeft} < ${sliderLeft}`);
            miniPlayerTest.verify(sliderRight < pillLeft, `${sliderRight} < ${pillLeft}`);
        }

        /**
         * Tests that a Volume change made elsewhere shows up on the slider.
         */
        function test_follow_volume_changes() {
            miniPlayer.volume = 50;
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visualValue, 50);
        }

        /**
         * Tests that the Volume changes continuously while dragging and changes arriving meanwhile are ignored.
         */
        function test_change_the_volume_while_dragging() {
            const slider = miniPlayerTest.child("volumeSlider");
            miniPlayerTest.mousePress(slider, slider.width / 4, slider.height / 2);
            miniPlayerTest.mouseMove(slider, slider.width / 2, slider.height / 2);
            miniPlayerTest.verify(volumeRequestedSpy.count > 0);
            miniPlayerTest.compare(volumeRequestedSpy.signalArguments[volumeRequestedSpy.count - 1][0], 30);
            const requestsBefore = volumeRequestedSpy.count;
            miniPlayerTest.mouseMove(slider, slider.width * 3 / 4, slider.height / 2);
            miniPlayerTest.verify(volumeRequestedSpy.count > requestsBefore);
            miniPlayerTest.compare(volumeRequestedSpy.signalArguments[volumeRequestedSpy.count - 1][0], 45);
            miniPlayer.volume = 10;
            miniPlayerTest.compare(Math.round(slider.visualValue), 45);
            miniPlayerTest.mouseRelease(slider, slider.width * 3 / 4, slider.height / 2);
            miniPlayerTest.compare(slider.visualValue, 10);
        }

        /**
         * Tests that the Volume requested by dragging stays within the range of the Renderer.
         */
        function test_keep_the_dragged_volume_within_the_range_of_the_renderer() {
            miniPlayer.volumeMinimum = 10;
            const slider = miniPlayerTest.child("volumeSlider");
            miniPlayerTest.mousePress(slider, slider.width / 2, slider.height / 2);
            miniPlayerTest.mouseMove(slider, -slider.width, slider.height / 2);
            miniPlayerTest.mouseMove(slider, slider.width * 2, slider.height / 2);
            miniPlayerTest.mouseRelease(slider, slider.width * 2, slider.height / 2);
            const volumes = volumeRequestedSpy.signalArguments.map(arguments => arguments[0]);
            miniPlayerTest.verify(volumes.includes(10), volumes);
            miniPlayerTest.verify(volumes.includes(60), volumes);
            miniPlayerTest.verify(volumes.every(volume => volume >= 10 && volume <= 60), volumes);
        }

        /**
         * Tests that the Mute button shows the "volume up" icon and the slider isn't dimmed while not muted.
         */
        function test_show_volume_up_while_not_muted() {
            const button = miniPlayerTest.child("muteButton");
            miniPlayerTest.compare(button.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/volume_up.svg"));
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").opacity, 1);
        }

        /**
         * Tests that the Mute button shows the "volume off" icon and the slider is dimmed but usable while muted.
         */
        function test_show_volume_off_and_dim_the_slider_while_muted() {
            miniPlayer.muted = true;
            const button = miniPlayerTest.child("muteButton");
            miniPlayerTest.compare(button.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/volume_off.svg"));
            const slider = miniPlayerTest.child("volumeSlider");
            miniPlayerTest.compare(slider.opacity, Theme.disabledOpacity);
            miniPlayerTest.compare(slider.interactive, true);
            miniPlayerTest.mouseClick(slider, slider.width / 2, slider.height / 2);
            miniPlayerTest.verify(volumeRequestedSpy.count > 0);
            miniPlayerTest.compare(muteRequestedSpy.count, 0);
        }

        /**
         * Tests that the Mute button asks to mute and to unmute.
         */
        function test_toggle_the_mute_with_the_mute_button() {
            const button = miniPlayerTest.child("muteButton");
            miniPlayerTest.verify(button.width >= 44 && button.height >= 44);
            miniPlayerTest.mouseClick(button);
            miniPlayerTest.compare(muteRequestedSpy.count, 1);
            miniPlayerTest.compare(muteRequestedSpy.signalArguments[0][0], true);
            miniPlayer.muted = true;
            miniPlayerTest.mouseClick(button);
            miniPlayerTest.compare(muteRequestedSpy.count, 2);
            miniPlayerTest.compare(muteRequestedSpy.signalArguments[1][0], false);
            miniPlayerTest.compare(nowPlayingRequestedSpy.count, 0);
        }

        /**
         * Tests that the Mute button is hidden without Mute control, the slider without Volume control and both
         * without either.
         */
        function test_hide_the_controls_that_the_renderer_does_not_offer() {
            miniPlayer.canControlMute = false;
            miniPlayerTest.compare(miniPlayerTest.child("muteButton").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visible, true);
            miniPlayer.canControlMute = true;
            miniPlayer.canControlVolume = false;
            miniPlayerTest.compare(miniPlayerTest.child("muteButton").visible, true);
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visible, false);
            miniPlayer.canControlMute = false;
            miniPlayerTest.compare(miniPlayerTest.child("volumeControls").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("muteButton").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visible, false);
        }

        /**
         * Tests that Mute and Volume are hidden without an Active Renderer.
         */
        function test_hide_mute_and_the_volume_without_an_active_renderer() {
            miniPlayer.hasActiveRenderer = false;
            miniPlayerTest.compare(miniPlayerTest.child("volumeControls").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("muteButton").visible, false);
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visible, false);
        }

        /**
         * Tests that Mute and Volume are kept usable while the Active Renderer has no media, as on the Playing screen.
         */
        function test_keep_mute_and_the_volume_without_media() {
            miniPlayer.playbackState = Renderer.NoMedia;
            miniPlayerTest.compare(miniPlayerTest.child("muteButton").visible, true);
            miniPlayerTest.compare(miniPlayerTest.child("volumeSlider").visible, true);
            miniPlayerTest.mouseClick(miniPlayerTest.child("muteButton"));
            miniPlayerTest.compare(muteRequestedSpy.count, 1);
        }
    }
}
