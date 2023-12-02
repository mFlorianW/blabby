// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 
import Blabby.Shell 1.0
import Blabby.Controls 1.0
import Blabby.Singleton 1.0

Item{
    id: shell
    anchors.fill: parent

    Rectangle{
        id: menuRow
        color: Theme.colors.surfaceContainerHeighest
        width: 150
        height: shell.height

        Rectangle{
            id: roundedStyle
            width: 100
            height: shell.height
            anchors.left: menuRow.horizontalCenter
            radius: 30
            color: Theme.colors.surface
        }

        Column{
            anchors.fill: parent
            anchors.top: menuRow.top
            anchors.topMargin: 30
            anchors.left: menuRow.left
            anchors.leftMargin: 12
            spacing: 40

            IconButton{
                id: burgerButton
                source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/burgermenu.svg"

                onClicked: {
                    if(menuWindow.x == -menuWindow.width){
                        moveIn.running = true
                        menuWindowHandler.enabled = true
                        blur.visible = true
                    }
                }
            }

            ListView{
                id: mediaSourceList
                width: 100
                height:400
                model: Singleton.mediaSourceModel
                delegate: IconButton{
                    source: mediaSourceIconUrl
                    onClicked: {
                        Singleton.mediaSourceModel.activateMediaSource(mediaSourceList.currentIndex)
                    }
                }
            }
        }
    }

    Rectangle{
        id: content
        color: Theme.colors.surface
        width: shell.width - menuRow.width - anchors.leftMargin
        height: shell.height
        anchors.left: menuRow.right
        anchors.leftMargin: -45

        Column{
            anchors.fill: parent
            Item{
                id: topControlArea
                width: parent.width
                height: 109

                Row{
                    anchors.top: topControlArea.top
                    anchors.topMargin: 40
                    width: parent.width
                    Item{
                        id: navigationPlaceHolder
                        height: topControlArea.height - 40
                        width: 200
                    }

                    Item{
                        id: searchBarPlaceHolder
                        height: topControlArea.height - 40
                        width: parent.width - navigationPlaceHolder.width - statusBar.width
                    }

                    Item{
                        id: statusBar
                        height: topControlArea.height - 40
                        width: 200
                        Row{
                            anchors.fill: parent
                            layoutDirection: Qt.RightToLeft
                            rightPadding: 20

                            Clock{
                                id: clock
                            }
                        }
                    }
                }

            }

            Item{
                id: mediaItemArea
                height: shell.height - topControlArea.height - mediaControlArea.height
                width: parent.width

                Header{
                    id: mediaItemModelHeader
                    height: 65
                    width: 765
                    text: "Music Box > ... > Bad Religion > True North"
                }

                ListView{
                    id: mediaItemListView
                    anchors.top: mediaItemModelHeader.bottom
                    width: mediaItemModelHeader.width
                    height: mediaItemModelHeader.height
                    model: Singleton.mediaItemModel
                    boundsBehavior: Flickable.StopAtBounds
                    delegate: ListEntry{
                        width: mediaItemListView.width
                        height: 64
                        title: mediaItemTitle
                    }
                }
            }

            Item{
                id: mediaControlArea
                width: parent.width
                height: 109
            }
        }
    }

    Menu{
        id: menuWindow
        height: shell.height
        width: 300
        visible: true
        x: -menuWindow.width

        NumberAnimation{
            id: moveIn
            target: menuWindow
            properties: "x"
            to: 0
            duration: 250
        }

        NumberAnimation{
            id: moveOut
            target: menuWindow
            properties: "x"
            to: -menuWindow.width
            duration: 250
        }
    }

    Rectangle{
        id: blur
        anchors.left: menuWindow.right
        anchors.right: shell.right
        height: shell.height
        color: Theme.stateColors.onSurfaceVariant.opacity08
        opacity: 0.08
        visible: false
        TapHandler{
            id: menuWindowHandler
            enabled: false
            onTapped: (event, button) => {
                moveOut.running = true
                menuWindowHandler.enabled = false
                blur.visible =  false
            }
        }
    }

    Connections{
        target: Singleton.mediaSourceModel
        function onActiveMediaSourceChanged() {
            Singleton.mediaItemModel.setMediaSource(Singleton.mediaSourceModel.activeMediaSource)
        }
    }
}
