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

        function init() {
            toast.duration = 200;
            toast.hide();
            toastTest.tryCompare(toast, "visible", false);
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
    }
}
