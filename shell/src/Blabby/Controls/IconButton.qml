// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A round button that only shows an icon, in the standard or the tonal variant.
 * A disabled icon button (enabled: false) is dimmed and emits no clicked signal.
 */
Item {
    id: iconButton

    /**
     * The look of the icon button.
     */
    enum Variant {
        /**
         * Only the icon, for actions next to other content.
         */
        Standard,
        /**
         * The icon on a filled container, for actions that shall stand out.
         */
        Tonal
    }

    /**
     * The variant of the icon button, a value of IconButton.Variant.
     */
    property int variant: IconButton.Standard

    /**
     * The URL of the SVG icon.
     */
    property url iconSource

    /**
     * The colour of the icon, depending on the variant.
     */
    readonly property color contentColor: iconButton.variant === IconButton.Tonal ? Theme.colors.colorOnSecondaryContainer : Theme.colors.colorOnSurfaceVariant

    /**
     * This signal is emitted when the icon button is clicked.
     */
    signal clicked

    // The touch target is bigger than the visible container.
    implicitWidth: 48
    implicitHeight: 48
    opacity: iconButton.enabled ? 1 : Theme.disabledOpacity

    Rectangle {
        id: container
        objectName: "container"
        anchors.centerIn: iconButton
        width: 40
        height: 40
        radius: container.width / 2
        color: iconButton.variant === IconButton.Tonal ? Theme.colors.secondaryContainer : "transparent"
    }

    Rectangle {
        id: stateLayer
        anchors.fill: container
        radius: container.radius
        color: iconButton.contentColor
        opacity: control.pressed ? Theme.stateLayer.pressedOpacity : control.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    AbstractInteractiveControl {
        id: control
        anchors.fill: iconButton
        onClicked: iconButton.clicked()
    }

    Icon {
        id: icon
        objectName: "icon"
        anchors.centerIn: iconButton
        width: 24
        height: 24
        source: iconButton.iconSource
        color: iconButton.contentColor
    }
}
