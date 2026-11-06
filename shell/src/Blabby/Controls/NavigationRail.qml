// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * A vertical bar at the left edge of the window that switches between the top level destinations.
 * Each entry of the model is an object with the properties "text" and "iconSource".
 * An entry with the property "atBottom" set to true is placed at the bottom edge of the rail instead of below the
 * other entries.
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

    /**
     * The entries of the model with their index in the model, split by their place in the rail.
     */
    readonly property var entries: rail.model.map((entry, index) => ({
                "entry": entry,
                "index": index
            }))
    readonly property var topEntries: rail.entries.filter(item => !item.entry.atBottom)
    readonly property var bottomEntries: rail.entries.filter(item => item.entry.atBottom === true)

    implicitWidth: 96
    color: Theme.colors.surface

    component Destination: NavigationRailItem {
        id: destination
        required property var modelData

        objectName: "railItem" + destination.modelData.index
        text: destination.modelData.entry.text
        iconSource: destination.modelData.entry.iconSource
        active: rail.currentIndex === destination.modelData.index
        onClicked: {
            rail.currentIndex = destination.modelData.index;
            rail.activated(destination.modelData.index);
        }
    }

    Column {
        id: topDestinations
        anchors.top: rail.top
        anchors.topMargin: 28
        anchors.horizontalCenter: rail.horizontalCenter
        spacing: 12

        Repeater {
            model: rail.topEntries

            Destination {}
        }
    }

    Column {
        id: bottomDestinations
        anchors.bottom: rail.bottom
        anchors.bottomMargin: 24
        anchors.horizontalCenter: rail.horizontalCenter
        spacing: 12

        Repeater {
            model: rail.bottomEntries

            Destination {}
        }
    }
}
