// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import Blabby.Shell 1.0
import Blabby.Controls 1.0

AbstractIconButton{
    id: iconButton

    implicitHeight: 48
    implicitWidth: 48

    Rectangle{
        id: stateLayer
        width: 40
        height: 40
        radius: width*0.5
        anchors.centerIn: parent
        color: "transparent"
    }

    Image{
        id: icon
        anchors.centerIn: iconButton
        source: iconButton.source
        width: 24
        height: 24
    }

    TapHandler{
        id: tapHandler

        onTapped: (event, button) => {
            iconButton.state = "clicked"
            iconButton.clicked()
        }
    }

    HoverHandler{
        id: hoverHandler

        onHoveredChanged: () => {
            if (hoverHandler.hovered){
                iconButton.state = "hovered"
            } else {
                iconButton.state = ""
            }
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