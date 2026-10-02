// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects
import Blabby.Controls
import Blabby.Theme

/**
 * The menu of a Playable or a Container in the Library, headed by its title with the rows "Play next" and
 * "Add to Queue". It drops down under the item it is opened for, aligned to its right edge, or opens above it when
 * there is no room below.
 * The menu fills the screen it belongs to: while it is open a transparent tap catcher covers the screen, a tap on it
 * closes the menu and neither the tap nor the hover reach anything beneath. The menu is closed at start and closes
 * when it becomes invisible, e.g. when the screen is left.
 */
Item {
    id: itemMenu

    /**
     * The item the menu drops down under, set by @ref open().
     */
    property Item anchorItem: null

    /**
     * The title of the Playable or the Container the menu is open for, shown as its header.
     */
    property string title

    /**
     * True while the menu is open.
     */
    property bool opened: false

    /**
     * This signal is emitted when the user picks "Play next".
     */
    signal playNextPicked

    /**
     * This signal is emitted when the user picks "Add to Queue".
     */
    signal addToQueuePicked

    /**
     * The height of a row of the menu.
     */
    readonly property real rowHeight: 56

    /**
     * The space between the menu and the item it is opened for.
     */
    readonly property real gap: 8

    /**
     * The bounds of the anchor item in the coordinates of the menu, updated when the anchor item or the menu moves or
     * changes its size.
     */
    readonly property rect anchorBounds: {
        const item = itemMenu.anchorItem;
        if (item === null) {
            return Qt.rect(itemMenu.width, 0, 0, 0);
        }
        // mapToItem() isn't bound to anything, so the geometry of the anchor item, of its parent that lays it out and
        // of the menu is read here to map it anew whenever one of them changes, and when the menu opens.
        const parent = item.parent;
        void [item.x, item.y, item.width, item.height, parent?.x, parent?.y, parent?.width, itemMenu.width, itemMenu.height, itemMenu.opened];
        const topLeft = item.mapToItem(itemMenu, 0, 0);
        return Qt.rect(topLeft.x, topLeft.y, item.width, item.height);
    }

    /**
     * Opens the menu for the Playable or the Container with the title, under the item.
     */
    function open(item: Item, title: string) {
        itemMenu.anchorItem = item;
        itemMenu.title = title;
        itemMenu.opened = true;
    }

    /**
     * Closes the menu.
     */
    function close() {
        itemMenu.opened = false;
    }

    onVisibleChanged: {
        if (!itemMenu.visible) {
            itemMenu.close();
        }
    }

    MouseArea {
        id: tapCatcher
        objectName: "itemMenuTapCatcher"
        anchors.fill: itemMenu
        visible: itemMenu.opened
        // Takes the hover, so nothing beneath shows a hover state while the menu is open.
        hoverEnabled: true
        onClicked: itemMenu.close()
        onWheel: wheel => wheel.accepted = true
    }

    Item {
        id: panel
        objectName: "itemMenuPanel"

        /**
         * True when the panel fits below the anchor item, it opens above it otherwise.
         */
        readonly property bool fitsBelow: itemMenu.anchorBounds.y + itemMenu.anchorBounds.height + itemMenu.gap + panel.height <= itemMenu.height

        x: Math.max(0, itemMenu.anchorBounds.x + itemMenu.anchorBounds.width - panel.width)
        y: panel.fitsBelow ? itemMenu.anchorBounds.y + itemMenu.anchorBounds.height + itemMenu.gap : Math.max(0, itemMenu.anchorBounds.y - itemMenu.gap - panel.height)
        width: 280
        height: menuColumn.height + 16
        visible: itemMenu.opened

        // Draws the shadow of the background, the background is drawn on top of it by itself.
        MultiEffect {
            anchors.fill: panelBackground
            source: panelBackground
            shadowEnabled: true
            shadowColor: Theme.colors.scrim
            shadowOpacity: 0.3
            shadowBlur: 0.4
            shadowVerticalOffset: 2
        }

        Rectangle {
            id: panelBackground
            anchors.fill: panel
            radius: 16
            color: Theme.colors.surfaceContainerHigh
        }

        // Taps and the hover on the panel outside of a row neither close the menu nor reach anything beneath.
        MouseArea {
            anchors.fill: panel
            hoverEnabled: true
        }

        Column {
            id: menuColumn
            anchors.top: panel.top
            anchors.topMargin: 8
            anchors.left: panel.left
            anchors.right: panel.right

            StyledText {
                objectName: "itemMenuHeader"
                width: menuColumn.width
                leftPadding: 16
                rightPadding: 16
                topPadding: 8
                bottomPadding: 4
                text: itemMenu.title
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.labelMedium
                elide: Text.ElideRight
                maximumLineCount: 1
            }

            Repeater {
                model: [
                    {
                        name: "playNextRow",
                        text: qsTr("Play next"),
                        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/queue_play_next.svg",
                        picked: itemMenu.playNextPicked
                    },
                    {
                        name: "addToQueueRow",
                        text: qsTr("Add to Queue"),
                        iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/playlist_add.svg",
                        picked: itemMenu.addToQueuePicked
                    }
                ]

                delegate: AbstractInteractiveControl {
                    id: menuRow
                    required property var modelData

                    objectName: menuRow.modelData.name
                    width: menuColumn.width
                    height: itemMenu.rowHeight
                    onClicked: {
                        itemMenu.close();
                        menuRow.modelData.picked();
                    }

                    Rectangle {
                        anchors.fill: menuRow
                        color: Theme.colors.colorOnSurface
                        opacity: menuRow.pressed ? Theme.stateLayer.pressedOpacity : menuRow.hovered ? Theme.stateLayer.hoverOpacity : 0
                    }

                    Icon {
                        id: rowIcon
                        anchors.left: menuRow.left
                        anchors.leftMargin: 16
                        anchors.verticalCenter: menuRow.verticalCenter
                        width: 24
                        height: 24
                        source: menuRow.modelData.iconSource
                        color: Theme.colors.colorOnSurfaceVariant
                    }

                    StyledText {
                        objectName: "label"
                        anchors.left: rowIcon.right
                        anchors.leftMargin: 16
                        anchors.right: menuRow.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: menuRow.verticalCenter
                        text: menuRow.modelData.text
                        color: Theme.colors.colorOnSurface
                        textStyle: Theme.fonts.bodyLarge
                        maximumLineCount: 1
                    }
                }
            }
        }
    }
}
