// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Shell 1.0
import Blabby.Controls 1.0
import Blabby.Singleton 1.0

Dialog{
    id: mediaRendererDialog
    title: "Select Playback Device"
    anchors.centerIn: shell
    contentItem: ListView{
        id: mediaItemListView
        model: Singleton.mediaRendererModel
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        delegate: ListEntry{
            id: listEntry
            width: mediaItemListView.width
            height: 64
            title: mediaRendererTitle
            iconUrl: mediaRendererIconUrl

            trailingElement: Switch{
                id: activeSwitch
                switchState: mediaRendererActive ? Switch.SwitchState.Active : Switch.SwitchState.Inactive
                onClicked: {
                    listEntry.activateRenderer()
                }
            }

            onClicked: {
                activateRenderer()
            }

            function activateRenderer(){
                Singleton.mediaRendererModel.activateRenderer(Singleton.mediaRendererModel.index(index, 0))
            }
        }
    }
}
