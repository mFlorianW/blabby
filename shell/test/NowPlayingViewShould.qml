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
