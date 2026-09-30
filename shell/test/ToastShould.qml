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

    Toast {
        id: toast
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
    }

    TestCase {
        id: toastTest
        name: "ToastShould"
        when: windowShown

        SignalSpy {
            id: actionClickedSpy
            target: toast
            signalName: "actionClicked"
        }

        function init() {
            toast.duration = 200;
            toast.hide();
            toastTest.tryCompare(toast, "visible", false);
            actionClickedSpy.clear();
        }

        /**
         * Tests that the toast is hidden until a message is shown.
         */
        function test_be_hidden_until_a_message_is_shown() {
            toastTest.compare(toast.visible, false);
        }

        /**
         * Tests that showing a message makes the toast visible with the message.
         */
        function test_show_the_message() {
            toast.show("Couldn't open Albums");
            toastTest.tryCompare(toast, "visible", true);
            toastTest.compare(toastTest.findChild(toast, "message").text, "Couldn't open Albums");
        }

        /**
         * Tests that the toast hides itself after its duration.
         */
        function test_hide_after_the_duration() {
            toast.show("Couldn't open Albums");
            toastTest.tryCompare(toast, "visible", true);
            toastTest.tryCompare(toast, "visible", false, 2000);
        }

        /**
         * Tests that a new message replaces the shown one and restarts the duration.
         */
        function test_replace_the_message_and_restart_the_duration() {
            toast.duration = 400;
            toast.show("Couldn't open Albums");
            toastTest.wait(300);
            toast.show("Couldn't open Artists");
            toastTest.compare(toastTest.findChild(toast, "message").text, "Couldn't open Artists");
            toastTest.wait(200);
            toastTest.compare(toast.visible, true);
            toastTest.tryCompare(toast, "visible", false, 2000);
        }

        /**
         * Tests that a message is shown without an action button unless an action is given.
         */
        function test_show_no_action_without_an_action_text() {
            toast.show("Couldn't open Albums");
            toastTest.compare(toastTest.findChild(toast, "action").visible, false);
        }

        /**
         * Tests that a message with an action shows the action button, clicking it notifies and hides the toast.
         */
        function test_show_an_action_and_notify_when_it_is_clicked() {
            toast.duration = 4000;
            toast.show("Choose a Renderer to play the Queue", "Choose");
            toastTest.tryCompare(toast, "visible", true);
            const action = toastTest.findChild(toast, "action");
            toastTest.compare(action.visible, true);
            toastTest.compare(action.text, "Choose");

            toastTest.mouseClick(action);

            toastTest.compare(actionClickedSpy.count, 1);
            toastTest.tryCompare(toast, "visible", false);
        }

        /**
         * Tests that a message without an action removes the action of the shown message.
         */
        function test_drop_the_action_of_a_replaced_message() {
            toast.show("Choose a Renderer to play the Queue", "Choose");
            toast.show("Couldn't open Albums");
            toastTest.compare(toastTest.findChild(toast, "action").visible, false);
        }
    }
}
