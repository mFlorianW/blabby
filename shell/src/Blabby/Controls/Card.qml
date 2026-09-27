// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A tappable outlined card that holds arbitrary content.
 * Items declared inside the card are placed in the content slot, inset by the padding.
 * A disabled card (enabled: false) is dimmed and emits no clicked signal.
 */
AbstractInteractiveControl {
    id: card

    /**
     * True when the card is the selected one, e.g. the Active Renderer.
     */
    property bool selected: false

    /**
     * The space between the edge of the card and its content.
     */
    property real padding: 16

    /**
     * The colour for the content of the card, depending on the selected state.
     */
    readonly property color contentColor: card.selected ? Theme.colors.colorOnSecondaryContainer : Theme.colors.colorOnSurface

    /**
     * The content of the card.
     */
    default property alias content: contentSlot.data

    /**
     * The item that holds the content, content items anchor to it.
     */
    readonly property alias contentItem: contentSlot

    opacity: card.enabled ? 1 : Theme.disabledOpacity

    Rectangle {
        id: container
        objectName: "container"
        anchors.fill: card
        radius: 12
        color: card.selected ? Theme.colors.secondaryContainer : Theme.colors.surfaceContainerLow
        border.color: card.selected ? Theme.colors.primary : Theme.colors.outlineVariant
        border.width: card.selected ? 2 : 1
    }

    Rectangle {
        id: stateLayer
        anchors.fill: container
        radius: container.radius
        color: card.contentColor
        opacity: card.pressed ? Theme.stateLayer.pressedOpacity : card.hovered ? Theme.stateLayer.hoverOpacity : 0
    }

    Item {
        id: contentSlot
        objectName: "content"
        anchors.fill: card
        anchors.margins: card.padding
    }
}
