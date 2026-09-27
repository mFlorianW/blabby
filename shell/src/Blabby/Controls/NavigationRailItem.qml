// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A destination of the @ref NavigationRail: an icon inside a pill with a label below it.
 * The pill is filled while the item is active.
 */
AbstractInteractiveControl {
    id: railItem

    /**
     * The label shown below the icon.
     */
    property string text

    /**
     * The URL of the SVG icon.
     */
    property url iconSource

    /**
     * True when the destination of the item is shown.
     */
    property bool active: false

    /**
     * The colour of the icon and the state layer, depending on the active state.
     */
    readonly property color contentColor: railItem.active ? Theme.colors.colorOnSecondaryContainer : Theme.colors.colorOnSurfaceVariant

    implicitWidth: 80
    implicitHeight: 56

    Rectangle {
        id: pill
        objectName: "activePill"
        width: 56
        height: 32
        radius: pill.height / 2
        anchors.top: railItem.top
        anchors.horizontalCenter: railItem.horizontalCenter
        color: Theme.colors.secondaryContainer
        visible: railItem.active
    }

    Rectangle {
        id: stateLayer
        anchors.fill: pill
        radius: pill.radius
        color: railItem.contentColor
        opacity: railItem.pressed ? Theme.stateLayer.pressedOpacity : railItem.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    Icon {
        id: icon
        anchors.centerIn: pill
        width: 24
        height: 24
        source: railItem.iconSource
        color: railItem.contentColor
    }

    StyledText {
        id: label
        objectName: "label"
        anchors.top: pill.bottom
        anchors.topMargin: 4
        anchors.horizontalCenter: railItem.horizontalCenter
        width: railItem.width
        horizontalAlignment: Text.AlignHCenter
        text: railItem.text
        color: railItem.active ? Theme.colors.colorOnSurface : Theme.colors.colorOnSurfaceVariant
        textStyle: Theme.fonts.labelMedium
    }
}
