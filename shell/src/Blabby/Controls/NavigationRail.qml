// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A vertical bar at the left edge of the window that switches between the top level destinations.
 * Each entry of the model is an object with the properties "text" and "iconSource".
 */
Rectangle {
    id: rail

    /**
     * The destinations shown in the rail.
     */
    property var model: []

    /**
     * The index of the active destination.
     * The rail sets it when the user clicks a destination, so it should not be bound by the user of the rail.
     */
    property int currentIndex: 0

    /**
     * This signal is emitted when the user activates the destination at index.
     */
    signal activated(int index)

    implicitWidth: 80
    color: Theme.colors.surface

    Column {
        id: destinations
        anchors.top: rail.top
        anchors.topMargin: 44
        anchors.horizontalCenter: rail.horizontalCenter
        spacing: 12

        Repeater {
            model: rail.model

            NavigationRailItem {
                id: destination
                required property var modelData
                required property int index

                objectName: "railItem" + destination.index
                text: destination.modelData.text
                iconSource: destination.modelData.iconSource
                active: rail.currentIndex === destination.index
                onClicked: {
                    rail.currentIndex = destination.index;
                    rail.activated(destination.index);
                }
            }
        }
    }
}
