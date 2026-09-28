// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import Blabby.Shell
import Blabby.Controls
import Blabby.Singleton
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
                "text": qsTr("Library"),
                "iconSource": "qrc:/qt/qml/Blabby/Shell/icons/material/library_music.svg"
            },
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

        LibraryView {
            id: libraryView
            sources: Singleton.mediaSourceModel
            items: Singleton.mediaItemModel
            hasActiveSource: Singleton.mediaItemModel.hasMediaSource
            activeSourceName: Singleton.mediaItemModel.mediaSourceName
            onSourcePicked: index => Singleton.mediaSourceModel.activateMediaSource(index)
            // Tiles aren't activated yet: the Library stays on the root Container until it can navigate back.
        }

        RenderersView {
            id: renderersView
            model: Singleton.mediaRendererModel
            scanning: Singleton.mediaRendererModel.scanning
            onRescanRequested: Singleton.mediaRendererModel.rescan()
            onActivated: index => Singleton.mediaRendererModel.activateRenderer(Singleton.mediaRendererModel.index(index, 0))
        }
    }
}
