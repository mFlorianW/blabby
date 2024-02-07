// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Shell 1.0
import Blabby.Controls 1.0
import Blabby.Theme 1.0

AbstractIconButton {
    id: iconButton

    implicitHeight: 48
    implicitWidth: 48

    Rectangle {
        id: background
        width: 40
        height: 40
        radius: width * 0.5
        anchors.centerIn: parent
        color: Theme.colors.surfaceContainerHeighest
        border.color: iconButton.border ? Theme.colors.outline : "transparent"
        border.width: iconButton.border ? 1 : 0
        visible: iconButton.style === AbstractIconButton.Style.Tonal
    }

    Rectangle {
        id: stateLayer
        width: 40
        height: 40
        radius: width * 0.5
        anchors.centerIn: parent
        color: "transparent"
        border.color: iconButton.border ? Theme.colors.outline : "transparent"
        border.width: iconButton.border ? 1 : 0
    }

    Image {
        id: icon
        anchors.centerIn: iconButton
        source: iconButton.source
        width: 24
        height: 24
    }

    onPressedChanged: () => {
        iconButton.state = "clicked";
    }

    onClicked: () => {
        iconButton.state = iconButton.hovered === true ? "hovered" : "normal";
    }

    states: [
        State {
            name: "hovered"
            when: iconButton.hovered === true
            PropertyChanges {
                target: stateLayer
                color: Theme.stateColors.onSurfaceVariant.opacity08
                opacity: 0.08
            }
        },
        State {
            name: "normal"
            //when: iconButton.controlState === AbstractIconButton.ControlState.Inactive // && iconButton.hovered === false
            PropertyChanges {
                target: stateLayer
                color: "transparent"
            }
        },
        State {
            name: "clicked"
            //when: iconButton.controlState === AbstractIconButton.ControlState.Active
            PropertyChanges {
                target: stateLayer
                color: Theme.stateColors.onSurfaceVariant.opacity12
                opacity: 0.12
            }
        }
    ]
}
