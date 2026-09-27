// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import Blabby.Shell
import Blabby.Controls
import Blabby.Theme

Rectangle {
    id: shell
    color: Theme.colors.surface

    NavigationRail {
        id: rail
        anchors.top: shell.top
        anchors.bottom: shell.bottom
        anchors.left: shell.left
        model: [
            {
                "text": qsTr("Renderers"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            }
        ]
    }

    StackLayout {
        id: destinations
        anchors.top: shell.top
        anchors.bottom: shell.bottom
        anchors.left: rail.right
        anchors.right: shell.right
        currentIndex: rail.currentIndex

        RenderersView {
            id: renderersView
        }
    }
}
