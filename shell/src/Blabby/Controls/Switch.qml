// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import Blabby.Shell
import Blabby.Controls
import Blabby.Singleton
import Blabby.Theme

AbstractSwitch{
    id: switchControl
    implicitHeight: 32
    implicitWidth: 52

    Rectangle{
        id: background
        anchors.fill: switchControl
        color: Theme.colors.surfaceContainerHeighest
        border.width: 2
        border.color: Theme.colors.outline
        radius: 20
    }

    Rectangle{
        id: switchDot
        height: 16
        width: switchDot.height
        radius: 20
        color: Theme.colors.outline
        anchors.left: background.left
        anchors.leftMargin: 5
        anchors.verticalCenter: switchControl.verticalCenter
    }

    states:[
        State{
            name: "normalState"
            when: switchControl.pressed === false && switchControl.switchState === AbstractSwitch.SwitchState.Inactive
            PropertyChanges{
                target: switchDot
                height: 16
                width: switchDot.height
                anchors.leftMargin: 5
            }
            PropertyChanges{
                target: background
                color: Theme.colors.surfaceContainerHeighest
                border.width: 2
            }
        },
        State{
            name: "inactivePressedState"
            when: switchControl.pressed === true && switchControl.switchState === AbstractSwitch.SwitchState.Inactive
            PropertyChanges{
                target: switchDot
                height: 32
                width: switchDot.height
                anchors.leftMargin: 0
            }
        },
        State{
            name: "activePressedState"
            when: switchControl.pressed === true && switchControl.switchState === AbstractSwitch.SwitchState.Active
            PropertyChanges{
                target: switchDot
                height: 32
                width: switchDot.height
                anchors.rightMargin: 0
                color: Theme.colors.colorOnPrimary
            }
            AnchorChanges{
                target:switchDot
                anchors.left: undefined
                anchors.right: background.right
            }
        },
        State{
            name: "activeState"
            when: switchControl.pressed === false && switchControl.switchState === AbstractSwitch.SwitchState.Active
            PropertyChanges{
                target: switchDot
                height: 24
                width: switchDot.height
                anchors.leftMargin: 0
                anchors.rightMargin: 4
                color: Theme.colors.colorOnPrimary
            }
            PropertyChanges{
                target: background
                color: Theme.colors.primary
                border.width: 0
            }
            AnchorChanges{
                target:switchDot
                anchors.left: undefined
                anchors.right: background.right
            }
        }
    ]

    transitions:[
        Transition{
            SequentialAnimation{
                PropertyAnimation{
                    targets: switchDot
                    properties: "height,width" 
                    easing.type: Easing.InOutQuad
                }
                AnchorAnimation{
                    targets: switchDot
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]
}
