// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Shell 1.0

AbstractListEntry{
    id: listEntry
    implicitHeight: 48

    Rectangle{
        id: stateLayer
        anchors.fill:parent
        color: "transparent"
    }

    Image {
        id: entryIcon
        source: listEntry.iconUrl
        width: 24
        height: 24
        anchors.left: listEntry.left
        anchors.verticalCenter: listEntry.verticalCenter
        anchors.leftMargin: 20
    }

    Text {
        id: entryTitle
        anchors.verticalCenter: listEntry.verticalCenter
        anchors.left: entryIcon.right
        anchors.leftMargin: 20
        width: listEntry.width - entryIcon.width - anchors.leftMargin - entryIcon.anchors.leftMargin
        text: listEntry.title
        font.family: Theme.fonts.bodyLarge.family
        font.pixelSize: Theme.fonts.bodyLarge.size
        color: Theme.fonts.bodyLarge.color
        elide: Text.ElideRight
    }

    HoverHandler{
        id: hoverHandler
        onHoveredChanged: () => {
            if (hoverHandler.hovered){
                listEntry.state = "hovered"
            } else {
                listEntry.state = ""
            }
        }
    }

    TapHandler{
        id: tapHandler

        onTapped: (event, button) => {
            listEntry.state = "clicked"
            listEntry.clicked()
        }
    }

    states:[
        State{
            name: "hovered"
            PropertyChanges{
                target: stateLayer
                color: Theme.stateColors.onSurfaceVariant.opacity08
                opacity: 0.08
            }
        },
        State{
            name: "clicked"
            PropertyChanges{
                target: stateLayer
                color: Theme.stateColors.onSurfaceVariant.opacity12
                opacity: 0.12
            }
        }
    ]
}
