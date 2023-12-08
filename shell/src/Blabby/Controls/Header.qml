// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.15
import Blabby.Controls 1.0
import Blabby.Shell 1.0

AbstractHeader{
    id: header

    Rectangle{
        id: background
        color: Theme.colors.surfaceContainerHigh
        radius: 10
        anchors.fill: header
    }

    Image {
        id: headerIcon
        source: header.iconUrl
        width: 48
        height: headerIcon.width
        anchors.left: header.left
        anchors.verticalCenter: header.verticalCenter
        anchors.leftMargin: 20
    }

    Text{
        id: headerText
        anchors.verticalCenter: header.verticalCenter
        anchors.left: header.left
        anchors.leftMargin: 80
        height: header.heigt
        font.family: Theme.fonts.titleMedium.family
        font.pixelSize: Theme.fonts.titleMedium.size
        font.bold: Theme.fonts.titleMedium.bold
        color: Theme.fonts.titleMedium.color
        text: header.text
    }
}
