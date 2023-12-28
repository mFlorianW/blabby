// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Shell
import Blabby.Controls
import Blabby.Singleton
import Blabby.Theme

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
                        Singleton.mediaSourceModel.activateMediaSource(index)
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
                    anchors.topMargin: 30
                    width: parent.width
                    Item{
                        id: navigationContainer
                        height: topControlArea.height - 40
                        width: 200

                        IconButton{
                            id: backNavigation
                            anchors.left: navigationContainer.left
                            border: true
                            source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/navigate_back.svg"
                            onClicked: Singleton.mediaItemModel.navigateBack()
                        }
                        IconButton{
                            id: nextNavigation
                            anchors.left: backNavigation.right
                            border: true
                            source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/navigate_next.svg"
                            onClicked: Singleton.mediaItemModel.navigateForward()
                        }
                    }

                    Item{
                        id: searchBarPlaceHolder
                        height: topControlArea.height - 40
                        width: parent.width - navigationContainer.width - statusBar.width
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
                    text: Singleton.mediaItemModel.mediaSourceName
                    iconUrl: Singleton.mediaItemModel.mediaSourceIconUrl
                }

                ListView{
                    id: mediaItemListView
                    anchors.top: mediaItemModelHeader.bottom
                    width: mediaItemModelHeader.width
                    height: mediaItemArea.height - mediaItemModelHeader.height
                    model: Singleton.mediaItemModel
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    delegate: ListEntry{
                        width: mediaItemListView.width
                        height: 64
                        title: mediaItemTitle
                        iconUrl: mediaItemIconUrl
                        onClicked: {
                            Singleton.mediaItemModel.activateMediaItem(index)
                        }
                    }
                }
            }

            Item{
                id: mediaControlArea
                width: parent.width
                height: 109

                IconButton{
                    id: rendererButton
                    source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/renderer.svg"
                    anchors.verticalCenter: mediaControlArea.verticalCenter
                    anchors.right: mediaControlArea.right
                    anchors.rightMargin: 242
                    onClicked: rendererDialog.visible = !rendererDialog.visible
                }
            }
        }
    }

    RendererDialog{
        id: rendererDialog
        visible: false
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

    Connections{
        target: Singleton.mediaRendererModel
        function onActiveRendererChanged(){
            Singleton.mediaPlayer.setRenderer(Singleton.mediaRendererModel.activeRenderer)
        }
    }

    Connections{
        target: Singleton.mediaItemModel
        function onPlayRequest(mediaItem){
            Singleton.mediaPlayer.play(mediaItem)
        }
    }
}
