/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
import QtQuick 2.0
import QtQuick.Controls 2.15
import de.blabby.shell 1.0
import "controls"

Item {
    id: shell
    anchors.fill: parent

    Row{
        id: mainWindow
        anchors.fill: parent

        Rectangle{
            id: leftPanel
            height: parent.height
            width: 250
            color: "#243242"

            Text{
                id: clockDisplay
                anchors.top: parent.top
                anchors.topMargin: 40
                text: Qt.formatTime(clock.currentTime , "hh:mm")
                font.pixelSize: 28
                color: "#dae1e7"
                anchors.horizontalCenter: parent.horizontalCenter

                Clock{
                    id:clock
                }
            }

            ListView{
                id: multimediaPluginsList
                anchors.top: clockDisplay.bottom
                anchors.topMargin: 80
                width: parent.width
                height: parent.height - clockDisplay.height
                model: g_mainWindow.model
                delegate: MultiMediaPluginButton{
                    width: parent.width
                    height: 80
                    buttonText: pluginName
                    opacity: pluginActive === true ? 1 : 0.6
                    iconSource: pluginIconUrl

                    onClicked: {
                        g_mainWindow.activatePlugin(index)
                    }
                }
            }
        }

        Column{
            id: contentColumn
            height: parent.height
            width: parent.width - leftPanel.width

            Rectangle{
                id: contentHeader
                color: "#00909e"
                height: 125
                width: parent.width

                Rectangle{
                    id: backButtonBackground
                    height: 58
                    width: 58
                    color: "#dae1e7"
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    radius: 100
                    opacity: 0.2

                    Image {
                        id: backbuttonArrow
                        source: "qrc:/shell/icon/BackButton.png"
                        anchors.centerIn: parent
                    }

                    MouseArea{
                        id: backButtonMouseArea
                        anchors.fill: parent

                        onClicked: {
                        }
                    }
                }

                Text {
                    id: pluginHeaderText
                    text: g_mainWindow.activePluginName
                    anchors.centerIn: parent
                    font.pixelSize: 32
                    color: "#dae1e7"
                }
            }

            Rectangle{
                id: content
                color: "#dae1e7"
                height: parent.height
                width: parent.width

                StackView {
                    id: stack
                    anchors.fill: parent
                    initialItem: activeView

                    pushEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to:1
                            duration: 200
                        }
                    }

                    pushExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to:0
                            duration: 200
                        }
                    }

                    popEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to:1
                            duration: 200
                        }
                    }

                    popExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to:0
                            duration: 200
                        }
                    }

                    Component{
                        id: activeView

                        Loader{
                            id: contentLoader
                            anchors.fill: parent
                            source: g_mainWindow.activePluginUrl
                        }
                    }
                }
            }
        }
    }
}
