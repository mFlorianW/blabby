// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
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
            width: mediaItemListView.width
            height: 64
            title: mediaRendererTitle
            iconUrl: mediaRendererIconUrl
            onClicked: {
                //Singleton.mediaItemModel.activateMediaItem(index)
            }
        }
    }
}
