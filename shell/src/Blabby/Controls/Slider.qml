// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls 1.0
import Blabby.Shell 1.0
import Blabby.Theme 1.0

AbstractSlider {
    id: slider

    implicitWidth: 100
    implicitHeight: valueCircle.height

    Rectangle {
        id: sliderBackground
        height: 4
        width: slider.width
        color: Theme.colors.surfaceContainerHeighest
        radius: 10
        anchors.verticalCenter: slider.verticalCenter
    }

    Rectangle {
        id: valueBackground
        height: 4
        width: slider.visualPosition
        color: Theme.colors.primary
        radius: 10
        anchors.verticalCenter: slider.verticalCenter
    }

    Rectangle {
        id: valueCircle
        x: slider.visualPosition
        height: 20
        width: height
        radius: 360
        color: Theme.colors.primary
        anchors.verticalCenter: slider.verticalCenter
    }
}
