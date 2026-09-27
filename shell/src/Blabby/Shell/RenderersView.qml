// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls

/**
 * The screen to choose the Renderer that plays the music.
 */
Item {
    id: renderersView

    ScreenHeader {
        id: header
        anchors.top: renderersView.top
        anchors.left: renderersView.left
        anchors.right: renderersView.right
        anchors.margins: 24
        title: qsTr("Renderers")
        subtitle: qsTr("Choose where your music plays")
    }
}
