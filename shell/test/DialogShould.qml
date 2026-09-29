// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Theme

Item {
    id: root
    width: 800
    height: 480

    Dialog {
        id: dialog
        anchors.fill: root
        title: "Forget 'Workshop'?"
        text: "Blabby no longer remembers the Renderer."
        acceptText: "Forget"
    }

    TestCase {
        id: dialogTest
        name: "DialogShould"
        when: windowShown

        SignalSpy {
            id: acceptedSpy
            target: dialog
            signalName: "accepted"
        }

        SignalSpy {
            id: rejectedSpy
            target: dialog
            signalName: "rejected"
        }

        function init() {
            dialog.text = "Blabby no longer remembers the Renderer.";
            dialog.acceptText = "Forget";
            dialog.open();
            acceptedSpy.clear();
            rejectedSpy.clear();
        }

        /**
         * Gives the child of the dialog with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = dialogTest.findChild(dialog, objectName);
            dialogTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the dialog shows its title, its text and the actions.
         */
        function test_show_the_title_the_text_and_the_actions() {
            dialogTest.compare(dialogTest.child("title").text, "Forget 'Workshop'?");
            dialogTest.compare(dialogTest.child("text").text, "Blabby no longer remembers the Renderer.");
            dialogTest.compare(dialogTest.child("rejectButton").text, "Cancel");
            dialogTest.compare(dialogTest.child("acceptButton").text, "Forget");
        }

        /**
         * Tests that the dialog shows no text when it has none.
         */
        function test_hide_the_text_when_it_is_empty() {
            dialog.text = "";
            dialogTest.compare(dialogTest.child("text").visible, false);
        }

        /**
         * Tests that the dialog shows no accept action when it has no accept text.
         */
        function test_hide_the_accept_action_without_accept_text() {
            dialog.acceptText = "";
            dialogTest.compare(dialogTest.child("acceptButton").visible, false);
        }

        /**
         * Tests that the accept action emits accepted and closes the dialog.
         */
        function test_accept_and_close_on_the_accept_action() {
            dialogTest.mouseClick(dialogTest.child("acceptButton"));
            dialogTest.compare(acceptedSpy.count, 1);
            dialogTest.compare(rejectedSpy.count, 0);
            dialogTest.compare(dialog.visible, false);
        }

        /**
         * Tests that the reject action emits rejected and closes the dialog.
         */
        function test_reject_and_close_on_the_reject_action() {
            dialogTest.mouseClick(dialogTest.child("rejectButton"));
            dialogTest.compare(rejectedSpy.count, 1);
            dialogTest.compare(acceptedSpy.count, 0);
            dialogTest.compare(dialog.visible, false);
        }

        /**
         * Tests that tapping the scrim beside the dialog rejects and closes it.
         */
        function test_reject_and_close_when_the_scrim_is_tapped() {
            dialogTest.mouseClick(dialog, 5, 5);
            dialogTest.compare(rejectedSpy.count, 1);
            dialogTest.compare(dialog.visible, false);
        }

        /**
         * Tests that tapping the dialog itself keeps it open.
         */
        function test_stay_open_when_the_dialog_is_tapped() {
            dialogTest.mouseClick(dialogTest.child("title"));
            dialogTest.compare(rejectedSpy.count, 0);
            dialogTest.compare(acceptedSpy.count, 0);
            dialogTest.compare(dialog.visible, true);
        }

        /**
         * Tests that the scrim and the dialog are drawn in the Theme colours.
         */
        function test_draw_the_scrim_and_the_dialog_in_theme_colours() {
            const scrim = dialogTest.child("scrim");
            dialogTest.verify(Qt.colorEqual(scrim.color, Theme.colors.scrim));
            dialogTest.compare(scrim.opacity, Theme.scrimOpacity);
            dialogTest.verify(Qt.colorEqual(dialogTest.child("panel").color, Theme.colors.surfaceContainerHigh));
        }

        /**
         * Tests that the dialog keeps the width of the design.
         */
        function test_keep_the_width_of_the_design() {
            const panel = dialogTest.child("panel");
            dialogTest.verify(panel.width >= 280);
            dialogTest.verify(panel.width <= 560);
        }

        /**
         * Tests that closing the dialog neither accepts nor rejects it.
         */
        function test_close_without_accepting_or_rejecting() {
            dialog.close();
            dialogTest.compare(dialog.visible, false);
            dialogTest.compare(acceptedSpy.count, 0);
            dialogTest.compare(rejectedSpy.count, 0);
        }

        /**
         * Tests that scrolling on the scrim doesn't reach the content below and keeps the dialog open.
         */
        function test_keep_scrolling_from_the_content_below() {
            const scrimArea = dialogTest.findChild(dialog, "scrimArea");
            dialogTest.verify(scrimArea);
            dialogTest.mouseWheel(dialog, 5, 5, 0, -120);
            dialogTest.compare(dialog.visible, true);
            dialogTest.compare(rejectedSpy.count, 0);
        }
    }
}
