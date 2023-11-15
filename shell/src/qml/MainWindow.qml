// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 
import BlabbyShell 0.0

Item{
    id: shell
    anchors.fill: parent

    Row{
        id: content
        anchors.fill: parent

        Rectangle{
            id: menuRow
            color: Theme.colors.surfaceContainerHeighest
            width: 125
            height: shell.height


            Rectangle{
                id: roundedStyle
                width: 85
                height: shell.height
                anchors.left: menuRow.horizontalCenter
                radius: 30
                color: Theme.colors.surface
            }
            Row{
                anchors.fill: parent
                anchors.top: menuRow.top
                anchors.topMargin: 30
                anchors.left: menuRow.left
                anchors.leftMargin: 8
                IconButton{
                    id: burgerButton
                    source: "qrc:/qt/qml/BlabbyShell/icons/24x24/burgermenu.svg"
                }
            }
        }

        Rectangle{
            id: contentRow
            color: Theme.colors.surface
            width: shell.width - menuRow.width
            height: shell.height

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
                }

                Item{
                    id: mediaControlArea
                    width: parent.width
                    height: 109
                }
            }
        }
    }
}
