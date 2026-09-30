// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Objects
import Blabby.Shell
import Blabby.Theme

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

        SignalSpy {
            id: queueRequestedSpy
            target: nowPlayingView
            signalName: "queueRequested"
        }

        SignalSpy {
            id: seekRequestedSpy
            target: nowPlayingView
            signalName: "seekRequested"
        }

        SignalSpy {
            id: volumeRequestedSpy
            target: nowPlayingView
            signalName: "volumeRequested"
        }

        SignalSpy {
            id: muteRequestedSpy
            target: nowPlayingView
            signalName: "muteRequested"
        }

        SignalSpy {
            id: togglePlaybackRequestedSpy
            target: nowPlayingView
            signalName: "togglePlaybackRequested"
        }

        SignalSpy {
            id: previousRequestedSpy
            target: nowPlayingView
            signalName: "previousRequested"
        }

        SignalSpy {
            id: nextRequestedSpy
            target: nowPlayingView
            signalName: "nextRequested"
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
            nowPlayingView.canPause = true;
            nowPlayingView.transitioning = false;
            nowPlayingView.position = 102000;
            nowPlayingView.hasDuration = true;
            nowPlayingView.duration = 271000;
            nowPlayingView.canSeek = true;
            nowPlayingView.cancelSeek();
            nowPlayingView.volume = 42;
            nowPlayingView.volumeMinimum = 0;
            nowPlayingView.volumeMaximum = 60;
            nowPlayingView.canControlVolume = true;
            nowPlayingView.muted = false;
            nowPlayingView.canControlMute = true;
            muteRequestedSpy.clear();
            seekRequestedSpy.clear();
            volumeRequestedSpy.clear();
            nowPlayingViewTest.child("toast").hide();
            chooseRendererRequestedSpy.clear();
            queueRequestedSpy.clear();
            togglePlaybackRequestedSpy.clear();
            nowPlayingView.hasQueue = true;
            nowPlayingView.hasPrevious = true;
            nowPlayingView.hasNext = true;
            previousRequestedSpy.clear();
            nextRequestedSpy.clear();
            // The actions of the header are laid out on the next polish, clicks before would miss them.
            nowPlayingViewTest.waitForItemPolished(nowPlayingViewTest.child("rendererPill").parent);
        }

        /**
         * Shows the Current Track in the Playback State and waits until its layout is done, so clicks hit.
         */
        function showTrack(playbackState) {
            nowPlayingView.playbackState = playbackState;
            nowPlayingViewTest.waitForRendering(nowPlayingView);
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
         * Tests that the Queue button in the header asks to open the Queue, also without an Active Renderer.
         */
        function test_ask_to_open_the_queue_with_the_queue_button() {
            const queueButton = nowPlayingViewTest.child("queueButton");
            nowPlayingViewTest.compare(queueButton.visible, true);
            nowPlayingViewTest.mouseClick(queueButton);
            nowPlayingViewTest.compare(queueRequestedSpy.count, 1);

            nowPlayingView.hasActiveRenderer = false;
            nowPlayingViewTest.compare(queueButton.visible, true);
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
         * Tests that the Play/Pause button shows the pause icon and the playing shape while Playing.
         */
        function test_show_the_pause_icon_while_playing() {
            nowPlayingView.playbackState = Renderer.Playing;
            const button = nowPlayingViewTest.child("playPauseButton");
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(button, "icon").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/pause.svg"));
            nowPlayingViewTest.tryCompare(nowPlayingViewTest.findChild(button, "container"), "radius", 28);
        }

        /**
         * Tests that the Play/Pause button shows the stop icon while Playing on a Renderer that can't pause.
         */
        function test_show_the_stop_icon_while_playing_on_a_renderer_that_cannot_pause() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.canPause = false;
            const button = nowPlayingViewTest.child("playPauseButton");
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(button, "icon").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/stop.svg"));
        }

        function test_show_the_play_icon_while_not_playing_data() {
            return [
                {
                    tag: "Paused",
                    state: Renderer.Paused
                },
                {
                    tag: "Stopped",
                    state: Renderer.Stopped
                }
            ];
        }

        /**
         * Tests that the Play/Pause button shows the play icon and the round shape while Paused or Stopped.
         */
        function test_show_the_play_icon_while_not_playing(data) {
            nowPlayingView.playbackState = data.state;
            const button = nowPlayingViewTest.child("playPauseButton");
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(button, "icon").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/play_arrow.svg"));
            nowPlayingViewTest.tryCompare(nowPlayingViewTest.findChild(button, "container"), "radius", 48);
        }

        /**
         * Tests that the Play/Pause button shows a busy ring while the Renderer is transitioning.
         */
        function test_show_a_busy_ring_while_transitioning() {
            nowPlayingView.playbackState = Renderer.Playing;
            const busyRing = nowPlayingViewTest.findChild(nowPlayingViewTest.child("playPauseButton"), "busyRing");
            nowPlayingViewTest.verify(busyRing);
            nowPlayingViewTest.compare(busyRing.visible, false);
            nowPlayingView.transitioning = true;
            nowPlayingViewTest.compare(busyRing.visible, true);
        }

        /**
         * Tests that the Play/Pause button asks to toggle the playback.
         */
        function test_ask_to_toggle_the_playback_with_the_play_pause_button() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const button = nowPlayingViewTest.child("playPauseButton");
            nowPlayingViewTest.verify(button.width >= 44 && button.height >= 44);
            nowPlayingViewTest.mouseClick(button);
            nowPlayingViewTest.compare(togglePlaybackRequestedSpy.count, 1);
        }

        function test_show_a_toast_when_a_control_call_failed_data() {
            return [
                {
                    tag: "Play",
                    action: Renderer.Play,
                    message: "Couldn't play Kitchen"
                },
                {
                    tag: "Resume",
                    action: Renderer.Resume,
                    message: "Couldn't resume Kitchen"
                },
                {
                    tag: "Pause",
                    action: Renderer.Pause,
                    message: "Couldn't pause Kitchen"
                },
                {
                    tag: "Stop",
                    action: Renderer.Stop,
                    message: "Couldn't stop Kitchen"
                },
                {
                    tag: "Seek",
                    action: Renderer.Seek,
                    message: "Couldn't seek on Kitchen"
                },
                {
                    tag: "ChangeVolume",
                    action: Renderer.ChangeVolume,
                    message: "Couldn't change the Volume of Kitchen"
                },
                {
                    tag: "Mute",
                    action: Renderer.Mute,
                    message: "Couldn't mute Kitchen"
                },
                {
                    tag: "Unmute",
                    action: Renderer.Unmute,
                    message: "Couldn't unmute Kitchen"
                }
            ];
        }

        /**
         * Tests that a toast tells which control call on which Renderer failed.
         */
        function test_show_a_toast_when_a_control_call_failed(data) {
            const toast = nowPlayingViewTest.child("toast");
            nowPlayingView.showControlFailed("Kitchen", data.action);
            nowPlayingViewTest.tryCompare(toast, "visible", true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(toast, "message").text, data.message);
        }

        /**
         * Tests that the seek bar shows the elapsed and the total time and advances with every position update.
         */
        function test_show_the_elapsed_and_the_total_time() {
            nowPlayingView.playbackState = Renderer.Playing;
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.compare(slider.visible, true);
            nowPlayingViewTest.compare(slider.value, 102000);
            nowPlayingViewTest.compare(slider.to, 271000);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").text, "1:42");
            nowPlayingViewTest.compare(nowPlayingViewTest.child("totalTime").text, "4:31");
            nowPlayingView.position = 103000;
            nowPlayingViewTest.compare(slider.value, 103000);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").text, "1:43");
        }

        /**
         * Tests that times of an hour and longer show the hours.
         */
        function test_show_the_hours_of_long_durations() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.duration = 3723000;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("totalTime").text, "1:02:03");
        }

        /**
         * Tests that dragging shows the target in the bubble, keeps the elapsed time and seeks once on release,
         * the bar stays at the target until the next position update.
         */
        function test_seek_once_on_release() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.compare(slider.interactive, true);
            nowPlayingViewTest.mousePress(slider, slider.width / 4, slider.height / 2);
            nowPlayingViewTest.mouseMove(slider, slider.width / 2, slider.height / 2);
            const indicator = nowPlayingViewTest.findChild(slider, "valueIndicator");
            nowPlayingViewTest.compare(indicator.visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(indicator, "valueIndicatorText").text, "2:15");
            nowPlayingView.position = 103000;
            nowPlayingViewTest.fuzzyCompare(slider.visualValue, 135500, 1000);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").text, "1:43");
            nowPlayingViewTest.compare(seekRequestedSpy.count, 0);
            nowPlayingViewTest.mouseRelease(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.compare(seekRequestedSpy.count, 1);
            const target = seekRequestedSpy.signalArguments[0][0];
            nowPlayingViewTest.fuzzyCompare(target, 135500, 1000);
            nowPlayingViewTest.compare(slider.visualValue, target);
            nowPlayingView.position = 136000;
            nowPlayingViewTest.compare(slider.visualValue, 136000);
        }

        /**
         * Tests that a tap on the seek bar seeks to that point.
         */
        function test_seek_with_a_tap() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.mouseClick(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.compare(seekRequestedSpy.count, 1);
            nowPlayingViewTest.fuzzyCompare(seekRequestedSpy.signalArguments[0][0], 135500, 1000);
        }

        /**
         * Tests that the seek bar snaps back and a toast is shown when the seek failed.
         */
        function test_snap_back_when_the_seek_failed() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.mouseClick(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.fuzzyCompare(slider.visualValue, 135500, 1000);
            nowPlayingView.showControlFailed("Kitchen", Renderer.Seek);
            nowPlayingViewTest.compare(slider.visualValue, 102000);
            const toast = nowPlayingViewTest.child("toast");
            nowPlayingViewTest.tryCompare(toast, "visible", true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(toast, "message").text, "Couldn't seek on Kitchen");
        }

        /**
         * Tests that the seek bar only shows the position when the Renderer can't seek.
         */
        function test_show_a_display_only_seek_bar_without_seeking() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.canSeek = false;
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.compare(slider.visible, true);
            nowPlayingViewTest.compare(slider.interactive, false);
        }

        /**
         * Tests that only the elapsed time is shown for a stream without a duration.
         */
        function test_show_only_the_elapsed_time_without_a_duration() {
            nowPlayingView.playbackState = Renderer.Playing;
            nowPlayingView.hasDuration = false;
            nowPlayingView.duration = 0;
            nowPlayingView.canSeek = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("seekSlider").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("totalTime").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").text, "1:42");
        }

        /**
         * Tests that a Stopped Renderer shows 0:00 of the total time and can't seek.
         */
        function test_show_the_start_without_seeking_while_stopped() {
            nowPlayingView.playbackState = Renderer.Stopped;
            const slider = nowPlayingViewTest.child("seekSlider");
            nowPlayingViewTest.compare(slider.value, 0);
            nowPlayingViewTest.compare(slider.interactive, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("elapsedTime").text, "0:00");
            nowPlayingViewTest.compare(nowPlayingViewTest.child("totalTime").text, "4:31");
        }

        /**
         * Tests that the Volume row shows the Volume in the range of the Renderer with the number next to it.
         */
        function test_show_the_volume_with_its_number() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeRow").visible, true);
            const slider = nowPlayingViewTest.child("volumeSlider");
            nowPlayingViewTest.compare(slider.from, 0);
            nowPlayingViewTest.compare(slider.to, 60);
            nowPlayingViewTest.compare(slider.value, 42);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeValue").text, "42");
        }

        /**
         * Tests that a Volume change made elsewhere shows up on the slider.
         */
        function test_follow_volume_changes() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingView.volume = 50;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeSlider").visualValue, 50);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeValue").text, "50");
        }

        /**
         * Tests that the Volume row is kept when the Active Renderer has no media.
         */
        function test_keep_the_volume_row_without_media() {
            nowPlayingViewTest.showTrack(Renderer.NoMedia);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("emptyState").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeRow").visible, true);
        }

        /**
         * Tests that the Volume row is hidden without Volume control or without an Active Renderer.
         */
        function test_hide_the_volume_row_without_volume_control() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingView.canControlVolume = false;
            nowPlayingView.canControlMute = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeRow").visible, false);
            nowPlayingView.canControlVolume = true;
            nowPlayingView.hasActiveRenderer = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeRow").visible, false);
        }

        /**
         * Tests that the Volume changes while dragging and changes arriving meanwhile are ignored.
         */
        function test_change_the_volume_while_dragging() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const slider = nowPlayingViewTest.child("volumeSlider");
            nowPlayingViewTest.mousePress(slider, slider.width / 4, slider.height / 2);
            nowPlayingViewTest.mouseMove(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.verify(volumeRequestedSpy.count > 0);
            nowPlayingViewTest.compare(volumeRequestedSpy.signalArguments[volumeRequestedSpy.count - 1][0], 30);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeValue").text, "30");
            nowPlayingView.volume = 10;
            nowPlayingViewTest.compare(Math.round(slider.visualValue), 30);
            nowPlayingViewTest.mouseRelease(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.compare(slider.visualValue, 10);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeValue").text, "10");
        }

        /**
         * Tests that the Mute button shows the "volume up" icon and the slider isn't dimmed while not muted.
         */
        function test_show_volume_up_while_not_muted() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const button = nowPlayingViewTest.child("muteButton");
            nowPlayingViewTest.compare(button.visible, true);
            nowPlayingViewTest.compare(button.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/volume_up.svg"));
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeSlider").opacity, 1);
        }

        /**
         * Tests that the Mute button shows the "volume off" icon and the slider is dimmed but usable while muted.
         */
        function test_show_volume_off_and_dim_the_slider_while_muted() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingView.muted = true;
            const button = nowPlayingViewTest.child("muteButton");
            nowPlayingViewTest.compare(button.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/volume_off.svg"));
            const slider = nowPlayingViewTest.child("volumeSlider");
            nowPlayingViewTest.verify(slider.opacity < 1);
            nowPlayingViewTest.compare(slider.interactive, true);
            nowPlayingViewTest.mouseClick(slider, slider.width / 2, slider.height / 2);
            nowPlayingViewTest.verify(volumeRequestedSpy.count > 0);
            nowPlayingViewTest.compare(muteRequestedSpy.count, 0);
        }

        /**
         * Tests that the Mute button asks to mute and to unmute.
         */
        function test_toggle_the_mute_with_the_mute_button() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const button = nowPlayingViewTest.child("muteButton");
            nowPlayingViewTest.verify(button.width >= 44 && button.height >= 44);
            nowPlayingViewTest.mouseClick(button);
            nowPlayingViewTest.compare(muteRequestedSpy.count, 1);
            nowPlayingViewTest.compare(muteRequestedSpy.signalArguments[0][0], true);
            nowPlayingView.muted = true;
            nowPlayingViewTest.mouseClick(button);
            nowPlayingViewTest.compare(muteRequestedSpy.count, 2);
            nowPlayingViewTest.compare(muteRequestedSpy.signalArguments[1][0], false);
        }

        /**
         * Tests that the Mute button is hidden without Mute control and the slider without Volume control.
         */
        function test_hide_the_controls_that_the_renderer_does_not_offer() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingView.canControlMute = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("muteButton").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeSlider").visible, true);
            nowPlayingView.canControlMute = true;
            nowPlayingView.canControlVolume = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeRow").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("muteButton").visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeSlider").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("volumeValue").visible, false);
        }

        /**
         * Tests that previous and next are shown next to the Play/Pause button while the Queue has entries.
         */
        function test_show_previous_and_next_while_the_queue_has_entries() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const previousButton = nowPlayingViewTest.child("previousButton");
            const nextButton = nowPlayingViewTest.child("nextButton");
            nowPlayingViewTest.compare(previousButton.visible, true);
            nowPlayingViewTest.compare(nextButton.visible, true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(previousButton, "icon").source, "qrc:/qt/qml/Blabby/Shell/icons/material/skip_previous.svg");
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(nextButton, "icon").source, "qrc:/qt/qml/Blabby/Shell/icons/material/skip_next.svg");
        }

        function test_show_previous_and_next_as_tonal_buttons_beside_play_pause_data() {
            return [
                {
                    tag: "previous",
                    button: "previousButton"
                },
                {
                    tag: "next",
                    button: "nextButton"
                }
            ];
        }

        /**
         * Tests that previous and next are large tonal buttons of the height of the Play/Pause button's row, as in the
         * design, with previous before and next after the Play/Pause button.
         */
        function test_show_previous_and_next_as_tonal_buttons_beside_play_pause(data) {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const button = nowPlayingViewTest.child(data.button);
            nowPlayingViewTest.compare(button.width, 80);
            nowPlayingViewTest.compare(button.height, 80);
            const container = nowPlayingViewTest.findChild(button, "container");
            nowPlayingViewTest.compare(container.radius, 24);
            nowPlayingViewTest.compare(container.color, Theme.colors.secondaryContainer);
            const icon = nowPlayingViewTest.findChild(button, "icon");
            nowPlayingViewTest.compare(icon.width, 32);
            nowPlayingViewTest.compare(icon.color, Theme.colors.colorOnSecondaryContainer);
            const playPause = nowPlayingViewTest.child("playPauseButton");
            const buttonX = button.mapToItem(nowPlayingView, 0, 0).x;
            const playPauseX = playPause.mapToItem(nowPlayingView, 0, 0).x;
            if (data.button === "previousButton") {
                nowPlayingViewTest.compare(playPauseX - (buttonX + button.width), 12);
            } else {
                nowPlayingViewTest.compare(buttonX - (playPauseX + playPause.width), 12);
            }
        }

        /**
         * Tests that previous and next are hidden while the Queue is empty.
         */
        function test_hide_previous_and_next_while_the_queue_is_empty() {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingView.hasQueue = false;
            nowPlayingViewTest.compare(nowPlayingViewTest.child("previousButton").visible, false);
            nowPlayingViewTest.compare(nowPlayingViewTest.child("nextButton").visible, false);
        }

        function test_ask_for_previous_and_next_with_their_buttons_data() {
            return [
                {
                    tag: "previous",
                    button: "previousButton",
                    spy: previousRequestedSpy,
                    otherSpy: nextRequestedSpy
                },
                {
                    tag: "next",
                    button: "nextButton",
                    spy: nextRequestedSpy,
                    otherSpy: previousRequestedSpy
                }
            ];
        }

        /**
         * Tests that the previous and the next button ask for previous and next.
         */
        function test_ask_for_previous_and_next_with_their_buttons(data) {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            nowPlayingViewTest.mouseClick(nowPlayingViewTest.child(data.button));
            nowPlayingViewTest.compare(data.spy.count, 1);
            nowPlayingViewTest.compare(data.otherSpy.count, 0);
        }

        function test_disable_previous_and_next_when_unavailable_data() {
            return [
                {
                    tag: "previous",
                    button: "previousButton",
                    availability: "hasPrevious",
                    spy: previousRequestedSpy
                },
                {
                    tag: "next",
                    button: "nextButton",
                    availability: "hasNext",
                    spy: nextRequestedSpy
                }
            ];
        }

        /**
         * Tests that previous and next are disabled and ask for nothing when they are unavailable.
         */
        function test_disable_previous_and_next_when_unavailable(data) {
            nowPlayingViewTest.showTrack(Renderer.Playing);
            const button = nowPlayingViewTest.child(data.button);
            nowPlayingViewTest.compare(button.enabled, true);
            nowPlayingView[data.availability] = false;
            nowPlayingViewTest.compare(button.enabled, false);
            nowPlayingViewTest.compare(button.visible, true);
            nowPlayingViewTest.mouseClick(button);
            nowPlayingViewTest.compare(data.spy.count, 0);
        }

        /**
         * Tests that a toast tells that the Active Renderer went Offline.
         */
        function test_show_a_toast_when_the_active_renderer_went_offline() {
            const toast = nowPlayingViewTest.child("toast");
            // A toast of a previous test fades out.
            nowPlayingViewTest.tryCompare(toast, "visible", false);
            nowPlayingView.hasActiveRenderer = false;
            nowPlayingView.rendererName = "";
            nowPlayingView.showActiveRendererWentOffline("Kitchen");
            nowPlayingViewTest.tryCompare(toast, "visible", true);
            nowPlayingViewTest.compare(nowPlayingViewTest.findChild(toast, "message").text, "Kitchen is no longer available");
            nowPlayingViewTest.compare(nowPlayingViewTest.child("emptyState").title, "Choose a Renderer");
        }
    }
}
