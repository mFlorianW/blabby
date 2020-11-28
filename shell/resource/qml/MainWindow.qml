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
import de.blabby.shell 1.0
import "qrc:/shell/qml/controls"

MainController {
    id: shell
    anchors.fill: parent
    model: mediaPluginModel
    pluginSource: fileSystemSource

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
                model: mediaPluginModel
                delegate: MultiMediaPluginButton{
                    width: parent.width
                    height: 29
                    buttonText: pluginName
                    active: pluginActive
                    iconSource: pluginIconUrl

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            shell.activatePlugin(index)
                        }
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

                Text {
                    id: pluginHeaderText
                    text: shell.activePluginName
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

                Loader{
                    id: contentLoader
                    anchors.fill: parent
                    source: shell.activePluginUrl
                }
            }

        }
    }

    MultimediaPluginModel{
        id: mediaPluginModel
    }

    FileSystemSource{
        id: fileSystemSource
    }
}
