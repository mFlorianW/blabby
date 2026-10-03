// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtTest
import Blabby.Objects
import Blabby.Shell

TestCase {
    id: controlFailureMessageTest
    name: "ControlFailureMessageShould"

    ControlFailureMessage {
        id: controlFailureMessage
    }

    function test_tell_which_control_call_on_which_renderer_failed_data() {
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
     * Tests that the message tells which control call on which Renderer failed.
     */
    function test_tell_which_control_call_on_which_renderer_failed(data) {
        controlFailureMessageTest.compare(controlFailureMessage.message("Kitchen", data.action), data.message);
    }
}
