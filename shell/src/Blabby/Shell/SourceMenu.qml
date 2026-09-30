// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects
import Blabby.Controls
import Blabby.Theme

/**
 * The menu to pick the Active Source that drops down under the Source pill, headed "Choose a Source" with a row per
 * Source that shows its icon in a round badge and its name.
 * The menu fills the screen it belongs to: while it is open a transparent tap catcher covers the screen, also the item
 * the menu drops down under, a tap on it closes the menu and neither the tap nor the hover reach anything beneath. There is no scrim. Sources that appear or vanish while the menu is open
 * show up in or vanish from it. The menu is closed at start and closes when it becomes invisible, e.g. when the screen
 * is left.
 */
Item {
    id: sourceMenu

    /**
     * The Sources to list, a model with the roles "mediaSourceName" and "mediaSourceIconUrl".
     */
    property alias model: sourceList.model

    /**
     * The number of Sources in the list.
     */
    readonly property alias count: sourceList.count

    /**
     * The item the menu drops down under, aligned to its right edge.
     */
    property Item anchorItem

    /**
     * True while the menu is open.
     */
    property bool opened: false

    /**
     * This signal is emitted when the user picks the Source at index.
     */
    signal picked(int index)

    /**
     * The height of a row of the list.
     */
    readonly property real rowHeight: 64

    /**
     * The bottom right corner of the anchor item in the coordinates of the menu, updated when the anchor item or the
     * menu moves or changes its size.
     */
    readonly property point anchorBottomRight: {
        const item = sourceMenu.anchorItem;
        if (item === null) {
            return Qt.point(sourceMenu.width, 0);
        }
        // mapToItem() isn't bound to anything, so the geometry of the anchor item, of its parent that lays it out and
        // of the menu is read here to map it anew whenever one of them changes, and when the menu opens.
        const parent = item.parent;
        void [item.x, item.y, item.width, item.height, parent?.x, parent?.y, parent?.width, sourceMenu.width, sourceMenu.height, sourceMenu.opened];
        return item.mapToItem(sourceMenu, item.width, item.height);
    }

    /**
     * Opens the menu.
     */
    function open() {
        sourceMenu.opened = true;
    }

    /**
     * Closes the menu.
     */
    function close() {
        sourceMenu.opened = false;
    }

    onVisibleChanged: {
        if (!sourceMenu.visible) {
            sourceMenu.close();
        }
    }

    MouseArea {
        id: tapCatcher
        objectName: "tapCatcher"
        anchors.fill: sourceMenu
        visible: sourceMenu.opened
        // Takes the hover, so nothing beneath shows a hover state while the menu is open.
        hoverEnabled: true
        onClicked: sourceMenu.close()
        onWheel: wheel => wheel.accepted = true
    }

    Item {
        id: panel
        objectName: "sourceMenuPanel"
        x: sourceMenu.anchorBottomRight.x - panel.width
        y: sourceMenu.anchorBottomRight.y + 8
        width: 380
        height: menuColumn.height + 16
        visible: sourceMenu.opened

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
            objectName: "sourceMenuBackground"
            anchors.fill: panel
            radius: 16
            color: Theme.colors.surfaceContainerHigh
        }

        // Taps and the hover on the panel outside of a row neither close the menu nor reach anything beneath.
        MouseArea {
            id: panelArea
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
                id: header
                objectName: "sourceMenuHeader"
                width: menuColumn.width
                leftPadding: 16
                rightPadding: 16
                topPadding: 8
                bottomPadding: 4
                text: qsTr("Choose a Source")
                color: Theme.colors.colorOnSurfaceVariant
                textStyle: Theme.fonts.labelMedium
            }

            ListView {
                id: sourceList
                objectName: "sourceList"
                width: menuColumn.width
                height: sourceList.count * sourceMenu.rowHeight
                interactive: false

                delegate: AbstractInteractiveControl {
                    id: sourceRow
                    required property int index
                    required property string mediaSourceName
                    required property string mediaSourceIconUrl

                    objectName: "sourceRow" + sourceRow.index
                    width: sourceList.width
                    height: sourceMenu.rowHeight
                    onClicked: {
                        sourceMenu.close();
                        sourceMenu.picked(sourceRow.index);
                    }

                    Rectangle {
                        id: stateLayer
                        anchors.fill: sourceRow
                        color: Theme.colors.colorOnSurface
                        opacity: sourceRow.pressed ? Theme.stateLayer.pressedOpacity : sourceRow.hovered ? Theme.stateLayer.hoverOpacity : 0
                    }

                    IconBadge {
                        id: sourceBadge
                        objectName: "sourceBadge" + sourceRow.index
                        anchors.left: sourceRow.left
                        anchors.leftMargin: 12
                        anchors.verticalCenter: sourceRow.verticalCenter
                        width: 40
                        height: 40
                        iconSource: sourceRow.mediaSourceIconUrl !== "" ? sourceRow.mediaSourceIconUrl : "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
                    }

                    StyledText {
                        id: sourceName
                        objectName: "sourceName" + sourceRow.index
                        anchors.left: sourceBadge.right
                        anchors.leftMargin: 12
                        anchors.right: sourceRow.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: sourceRow.verticalCenter
                        text: sourceRow.mediaSourceName
                        color: Theme.colors.colorOnSurface
                        textStyle: Theme.fonts.bodyLarge
                        maximumLineCount: 1
                    }
                }
            }
        }
    }
}
