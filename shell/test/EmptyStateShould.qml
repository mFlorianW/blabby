// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

Item {
    id: root
    width: 600
    height: 400

    EmptyState {
        id: emptyState
        anchors.fill: parent
        title: "No renderers found"
        hint: "Make sure your speakers are switched on"
        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
    }

    TestCase {
        id: emptyStateTest
        name: "EmptyStateShould"
        when: windowShown

        SignalSpy {
            id: actionClickedSpy
            target: emptyState
            signalName: "actionClicked"
        }

        function init() {
            emptyState.busy = false;
            emptyState.hint = "Make sure your speakers are switched on";
            emptyState.actionText = "";
            actionClickedSpy.clear();
        }

        /**
         * Gives the child of the empty state with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = emptyStateTest.findChild(emptyState, objectName);
            emptyStateTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the title and the hint are shown.
         */
        function test_show_the_title_and_the_hint() {
            emptyStateTest.compare(emptyStateTest.child("title").text, "No renderers found");
            emptyStateTest.compare(emptyStateTest.child("hint").text, "Make sure your speakers are switched on");
            emptyStateTest.compare(emptyStateTest.child("hint").visible, true);
        }

        /**
         * Tests that an empty hint is hidden.
         */
        function test_hide_an_empty_hint() {
            emptyState.hint = "";
            emptyStateTest.compare(emptyStateTest.child("hint").visible, false);
        }

        /**
         * Tests that a busy empty state shows a busy indicator in place of the icon.
         */
        function test_show_a_busy_indicator_while_busy() {
            emptyStateTest.compare(emptyStateTest.child("icon").visible, true);
            emptyStateTest.compare(emptyStateTest.child("busyIndicator").visible, false);
            emptyState.busy = true;
            emptyStateTest.compare(emptyStateTest.child("icon").visible, false);
            emptyStateTest.compare(emptyStateTest.child("busyIndicator").visible, true);
            emptyStateTest.compare(emptyStateTest.child("busyIndicator").running, true);
        }

        /**
         * Tests that no action button is shown without an action text.
         */
        function test_hide_the_action_without_an_action_text() {
            emptyStateTest.compare(emptyStateTest.child("action").visible, false);
        }

        /**
         * Tests that the action button shows the action text and emits actionClicked when clicked.
         */
        function test_emit_actionClicked_when_the_action_is_clicked() {
            emptyState.actionText = "Choose a Source";
            const action = emptyStateTest.child("action");
            emptyStateTest.compare(action.visible, true);
            emptyStateTest.compare(action.text, "Choose a Source");
            emptyStateTest.compare(action.variant, Button.Filled);
            emptyStateTest.verify(action.height >= 44);
            emptyStateTest.waitForRendering(emptyState);
            emptyStateTest.mouseClick(action);
            emptyStateTest.compare(actionClickedSpy.count, 1);
        }

        /**
         * Tests that the content is centred in the empty state.
         */
        function test_centre_the_content() {
            const title = emptyStateTest.child("title");
            const centre = title.mapToItem(emptyState, title.width / 2, 0);
            emptyStateTest.compare(Math.round(centre.x), emptyState.width / 2);
        }
    }
}
