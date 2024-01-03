// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Singleton 1.0
import Blabby.Theme 1.0

Rectangle{
    id: menu

    implicitWidth: 300
    color: Theme.colors.surface

    Rectangle{
        id: header
        height: 125
        width: menu.width
        color: Theme.colors.surfaceContainerHeighest
    }

    ListView{
        id: provdiderList
        anchors.top: header.bottom
        height: menu.height
        width: menu.width
        model: Singleton.mediaSourceModel
        boundsBehavior: Flickable.StopAtBounds
        delegate: ListEntry{
            width: provdiderList.width
            height: 48
            iconUrl: mediaSourceIconUrl
            title: mediaSourceName
            onClicked:{
                Singleton.mediaSourceModel.activateMediaSource(index)
            }
        }
    }
}
