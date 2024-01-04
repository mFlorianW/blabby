// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Shell 1.0
import Blabby.Theme 1.0

AbstractDialog {
    id: dialog
    implicitWidth: 675
    implicitHeight: 360

    // Do not hove Elements behind the dialog.
    HoverHandler {
        id: hoverEventConsumer
        blocking: true
    }

    Rectangle {
        id: background
        color: Theme.colors.surface
        border.color: Theme.colors.outline
        border.width: 1
        anchors.fill: dialog
    }

    Rectangle {
        id: header
        height: dialog.headerHeight
        anchors.top: dialog.top
        anchors.topMargin: 1
        anchors.left: dialog.left
        anchors.leftMargin: 1
        anchors.right: dialog.right
        anchors.rightMargin: 1
        color: Theme.colors.surfaceContainerHigh

        // Do not hove Elements behind the dialog.
        HoverHandler {
            id: hoverEventConsumerHeader
            blocking: true
        }

        TapHandler {
            id: tapEventConumserHeader
            gesturePolicy: TapHandler.ReleaseWithinBounds
        }

        Text {
            id: headerText
            anchors.verticalCenter: header.verticalCenter
            anchors.left: header.left
            anchors.leftMargin: 17
            height: header.heigt
            font.family: Theme.fonts.titleLarge.family
            font.pixelSize: Theme.fonts.titleLarge.size
            font.bold: Theme.fonts.titleLarge.bold
            color: Theme.fonts.titleLarge.color
            text: dialog.title
        }

        IconButton {
            id: closeButton
            source: "qrc:/qt/qml/Blabby/Shell/icons/24x24/close.svg"
            anchors.verticalCenter: header.verticalCenter
            anchors.right: header.right
            anchors.rightMargin: 10
            onClicked: dialog.visible = false
        }
    }

    FadeBehavior on visible {
        id: fadeBehavior
    }
}
