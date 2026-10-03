// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects
import Blabby.Theme

/**
 * The shell of a menu that drops down under an item, aligned to its right edge. The children of the shell are its
 * content, laid out in a column on a rounded, shadowed panel with 8 px above and below.
 * The shell fills the screen it belongs to: while it is open a transparent tap catcher covers the screen, also the item
 * the menu drops down under, a tap on it closes the menu and neither the tap, the hover nor the wheel reach anything
 * beneath. There is no scrim. The menu is closed at start and closes when it becomes invisible, e.g. when the screen
 * is left.
 */
Item {
    id: menuShell

    /**
     * The content of the menu, laid out top to bottom on the panel.
     */
    default property alias content: contentColumn.data

    /**
     * The panel of the menu, e.g. to give it an objectName.
     */
    readonly property alias panel: panel

    /**
     * The width of the panel.
     */
    property real panelWidth: 280

    /**
     * The width of the content, the width of the panel.
     */
    readonly property alias contentWidth: contentColumn.width

    /**
     * The space between the menu and the item it drops down under, and above and below the content.
     */
    readonly property real gap: 8

    /**
     * The item the menu drops down under, aligned to its right edge.
     */
    property Item anchorItem: null

    /**
     * True while the menu is open.
     */
    property bool opened: false

    /**
     * The bottom right corner of the anchor item in the coordinates of the menu, updated when the anchor item or the
     * menu moves or changes its size.
     */
    readonly property point anchorBottomRight: {
        const item = menuShell.anchorItem;
        if (item === null) {
            return Qt.point(menuShell.width, 0);
        }
        // mapToItem() isn't bound to anything, so the geometry of the anchor item, of its parent that lays it out and
        // of the menu is read here to map it anew whenever one of them changes, and when the menu opens.
        const parent = item.parent;
        void [item.x, item.y, item.width, item.height, parent?.x, parent?.y, parent?.width, menuShell.width, menuShell.height, menuShell.opened];
        return item.mapToItem(menuShell, item.width, item.height);
    }

    /**
     * Opens the menu.
     */
    function open() {
        menuShell.opened = true;
    }

    /**
     * Closes the menu.
     */
    function close() {
        menuShell.opened = false;
    }

    onVisibleChanged: {
        if (!menuShell.visible) {
            menuShell.close();
        }
    }

    MouseArea {
        id: tapCatcher
        objectName: "tapCatcher"
        anchors.fill: menuShell
        visible: menuShell.opened
        // Takes the hover, so nothing beneath shows a hover state while the menu is open.
        hoverEnabled: true
        onClicked: menuShell.close()
        onWheel: wheel => wheel.accepted = true
    }

    Item {
        id: panel
        objectName: "menuPanel"
        x: menuShell.anchorBottomRight.x - panel.width
        y: menuShell.anchorBottomRight.y + menuShell.gap
        width: menuShell.panelWidth
        height: contentColumn.height + 2 * menuShell.gap
        visible: menuShell.opened

        // Draws the shadow of the background, the background is drawn on top of it by itself.
        MultiEffect {
            id: panelShadow
            objectName: "panelShadow"
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
            objectName: "panelBackground"
            anchors.fill: panel
            radius: 16
            color: Theme.colors.surfaceContainerHigh
        }

        // Taps and the hover on the panel outside of the content neither close the menu nor reach anything beneath.
        MouseArea {
            id: panelArea
            anchors.fill: panel
            hoverEnabled: true
        }

        Column {
            id: contentColumn
            anchors.top: panel.top
            anchors.topMargin: menuShell.gap
            anchors.left: panel.left
            anchors.right: panel.right
        }
    }
}
