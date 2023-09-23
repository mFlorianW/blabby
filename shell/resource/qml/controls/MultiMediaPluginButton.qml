// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.0

Item {
    id: root

    property alias buttonText: buttonText.text
    property alias iconSource: pluginIcon.source
    signal clicked

    Rectangle{
        id: hover
        anchors.fill: parent
        color: "#00909e"
        visible: false
    }

    Row{
        id: buttonRow
        anchors.fill: parent
        anchors.leftMargin: 20
        spacing: 10


        Image {
            id: pluginIcon
            width: 29
            height: 29
            opacity: root.opacity
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: buttonText
            font.pixelSize: 18
            color: "#dae1e7"
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    MouseArea{
        id: buttonClicked
        anchors.fill: parent
        onClicked: root.clicked()

        onHoveredChanged: {
            hover.visible = hover.visible === false ? true : false
        }
    }
}
