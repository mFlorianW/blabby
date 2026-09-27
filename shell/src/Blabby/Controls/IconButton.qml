// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
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
        color: Theme.colors.surfaceContainerHighest
        visible: iconButton.style === AbstractIconButton.Style.Tonal
    }

    Rectangle {
        id: stateLayer
        width: 40
        height: 40
        radius: width * 0.5
        anchors.centerIn: parent
        color: Theme.colors.colorOnSurfaceVariant
        opacity: 0
    }

    Rectangle {
        id: outline
        width: 40
        height: 40
        radius: width * 0.5
        anchors.centerIn: parent
        color: "transparent"
        border.color: Theme.colors.outline
        border.width: 1
        visible: iconButton.border
    }

    Icon {
        id: icon
        anchors.centerIn: iconButton
        source: iconButton.source
        color: Theme.colors.colorOnSurfaceVariant
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
                color: Theme.colors.colorOnSurfaceVariant
                opacity: Theme.stateLayer.hoverOpacity
            }
        },
        State {
            name: "normal"
            //when: iconButton.controlState === AbstractIconButton.ControlState.Inactive // && iconButton.hovered === false
            PropertyChanges {
                target: stateLayer
                opacity: 0
            }
        },
        State {
            name: "clicked"
            //when: iconButton.controlState === AbstractIconButton.ControlState.Active
            PropertyChanges {
                target: stateLayer
                color: Theme.colors.colorOnSurfaceVariant
                opacity: Theme.stateLayer.pressedOpacity
            }
        }
    ]
}
