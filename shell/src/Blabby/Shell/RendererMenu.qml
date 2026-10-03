// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Objects
import Blabby.Theme

/**
 * The menu to pick the Active Renderer that drops down under or opens upwards above a Renderer pill, headed "Choose a
 * Renderer" with a row per Renderer that shows the speaker icon in a round badge and its name. Offline Renderers are
 * dimmed like on the Renderer card and ignore taps, the row of the Active Renderer is tinted. Without Renderers a
 * disabled "No Renderers found" line is shown. Renderers that appear or change their Availability while the menu is
 * open update in it. When the rows don't fit into the space towards the edge of the screen the list scrolls inside.
 * The tap catcher, the panel, the anchoring and opening and closing come from @ref MenuShell.
 */
MenuShell {
    id: rendererMenu

    /**
     * The Renderers to list, a model with the roles "name", "availability" and "active".
     */
    property alias model: rendererList.model

    /**
     * The number of Renderers in the list.
     */
    readonly property alias count: rendererList.count

    /**
     * This signal is emitted when the user picks the Renderer at index that isn't the Active Renderer.
     */
    signal picked(int index)

    /**
     * The height of a row of the list.
     */
    readonly property real rowHeight: 64

    panel.objectName: "rendererMenuPanel"
    panelWidth: 320

    // A list scrolled the last time starts at its first row again.
    onOpenedChanged: {
        if (rendererMenu.opened) {
            rendererList.positionViewAtBeginning();
        }
    }

    StyledText {
        id: header
        objectName: "rendererMenuHeader"
        width: rendererMenu.contentWidth
        leftPadding: 16
        rightPadding: 16
        topPadding: 8
        bottomPadding: 4
        text: qsTr("Choose a Renderer")
        color: Theme.colors.colorOnSurfaceVariant
        textStyle: Theme.fonts.labelMedium
    }

    StyledText {
        id: noRenderersLine
        objectName: "noRenderersLine"
        width: rendererMenu.contentWidth
        height: rendererMenu.rowHeight
        leftPadding: 16
        rightPadding: 16
        verticalAlignment: Text.AlignVCenter
        text: qsTr("No Renderers found")
        color: Theme.colors.colorOnSurface
        opacity: Theme.disabledOpacity
        textStyle: Theme.fonts.bodyLarge
        visible: rendererList.count === 0
    }

    ListView {
        id: rendererList
        objectName: "rendererList"

        /**
         * The height of all rows.
         */
        readonly property real rowsHeight: rendererList.count * rendererMenu.rowHeight

        width: rendererMenu.contentWidth
        height: Math.min(rendererList.rowsHeight, Math.max(0, rendererMenu.maximumContentHeight - header.height))
        interactive: rendererList.rowsHeight > rendererList.height
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        visible: rendererList.count > 0

        delegate: AbstractInteractiveControl {
            id: rendererRow
            required property int index
            required property string name
            required property int availability
            required property bool active

            objectName: "rendererRow" + rendererRow.index
            width: rendererList.width
            height: rendererMenu.rowHeight
            enabled: rendererRow.availability === Renderer.Online
            opacity: rendererRow.enabled ? 1 : Theme.disabledOpacity
            onClicked: {
                rendererMenu.close();
                if (!rendererRow.active) {
                    rendererMenu.picked(rendererRow.index);
                }
            }

            Rectangle {
                id: rowBackground
                objectName: "rendererRowBackground" + rendererRow.index
                anchors.fill: rendererRow
                color: Theme.colors.secondaryContainer
                visible: rendererRow.active
            }

            Rectangle {
                id: stateLayer
                anchors.fill: rendererRow
                color: Theme.colors.colorOnSurface
                opacity: rendererRow.pressed ? Theme.stateLayer.pressedOpacity : rendererRow.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            IconBadge {
                id: rendererBadge
                objectName: "rendererBadge" + rendererRow.index
                anchors.left: rendererRow.left
                anchors.leftMargin: 12
                anchors.verticalCenter: rendererRow.verticalCenter
                width: 40
                height: 40
                iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            }

            StyledText {
                id: rendererName
                objectName: "rendererName" + rendererRow.index
                anchors.left: rendererBadge.right
                anchors.leftMargin: 12
                anchors.right: rendererRow.right
                anchors.rightMargin: 16
                anchors.verticalCenter: rendererRow.verticalCenter
                text: rendererRow.name
                color: rendererRow.active ? Theme.colors.colorOnSecondaryContainer : Theme.colors.colorOnSurface
                textStyle: Theme.fonts.bodyLarge
                maximumLineCount: 1
            }
        }
    }
}
