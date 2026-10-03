// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQml
import Blabby.Objects

/**
 * The message that tells the user that a control call to a Renderer failed.
 */
QtObject {
    /**
     * Gives the message that the call of the action to the Renderer with the name failed.
     * @param action The action of the failed call, a value of Renderer.Action.
     */
    function message(rendererName: string, action: int): string {
        const messages = {
            [Renderer.Play]: qsTr("Couldn't play %1"),
            [Renderer.Resume]: qsTr("Couldn't resume %1"),
            [Renderer.Pause]: qsTr("Couldn't pause %1"),
            [Renderer.Stop]: qsTr("Couldn't stop %1"),
            [Renderer.Seek]: qsTr("Couldn't seek on %1"),
            [Renderer.ChangeVolume]: qsTr("Couldn't change the Volume of %1"),
            [Renderer.Mute]: qsTr("Couldn't mute %1"),
            [Renderer.Unmute]: qsTr("Couldn't unmute %1")
        };
        return messages[action].arg(rendererName);
    }
}
