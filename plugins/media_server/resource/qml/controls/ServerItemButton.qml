// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.12

Rectangle {
    id:root
    color: "#87949f"

    property alias text : itemText.text
    property alias iconSource : itemIcon.source
    signal clicked

    Row {
        id: contentRow
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        spacing: 25

        Image {
            id: itemIcon
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: itemText
            font.pixelSize: 30
            color: "#dae1e7"
            anchors.verticalCenter: parent.verticalCenter
        }

    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
