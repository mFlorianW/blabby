// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * The menu to pick the Active Source that drops down under the Source pill, headed "Choose a Source" with a row per
 * Source that shows its icon in a round badge and its name, or a disabled "No Sources found" line without any. The row
 * of the Active Source is highlighted, its badge is drawn in the highlighted colours and it carries a checkmark.
 * Sources that appear or vanish while the menu is open show up in or vanish from it. Below a divider the menu ends
 * with a "Rescan network" entry, a tap on it asks for a Rescan and keeps the menu open, so that the Sources the Rescan
 * finds show up in it. The menu grows with its rows until it reaches 24 px above the bottom of the shell, then its rows
 * scroll while the header and the entry stay in place. The tap catcher, the panel, the anchoring and opening and
 * closing come from @ref MenuShell.
 */
MenuShell {
    id: sourceMenu

    /**
     * The Sources to list, a model with the roles "mediaSourceName", "mediaSourceIconUrl" and "active".
     */
    property alias model: sourceList.model

    /**
     * The number of Sources in the list.
     */
    readonly property alias count: sourceList.count

    /**
     * True while a Rescan is running, the "Rescan network" entry shows a busy indicator and can't be tapped then.
     */
    property bool scanning: false

    /**
     * This signal is emitted when the user picks the Source at index that isn't the Active Source.
     */
    signal picked(int index)

    /**
     * This signal is emitted when the user taps "Rescan network" while no Rescan is running.
     */
    signal rescanRequested

    /**
     * The height of a row of the list.
     */
    readonly property real rowHeight: 64

    panel.objectName: "sourceMenuPanel"
    panelWidth: 380
    edgeMargin: 24

    // A list scrolled the last time starts at its first row again.
    onOpenedChanged: {
        if (sourceMenu.opened) {
            sourceList.positionViewAtBeginning();
        }
    }

    StyledText {
        id: menuHeader
        objectName: "sourceMenuHeader"
        width: sourceMenu.contentWidth
        leftPadding: 16
        rightPadding: 16
        topPadding: 8
        bottomPadding: 4
        text: qsTr("Choose a Source")
        color: Theme.colors.colorOnSurfaceVariant
        textStyle: Theme.fonts.labelMedium
    }

    // Without Sources the menu says so, so that tapping the pill never feels broken.
    StyledText {
        id: noSourcesLine
        objectName: "noSourcesLine"
        width: sourceMenu.contentWidth
        height: sourceMenu.rowHeight
        leftPadding: 16
        rightPadding: 16
        verticalAlignment: Text.AlignVCenter
        text: qsTr("No Sources found")
        color: Theme.colors.colorOnSurface
        textStyle: Theme.fonts.bodyLarge
        opacity: Theme.disabledOpacity
        visible: sourceList.count === 0
    }

    ListView {
        id: sourceList
        objectName: "sourceList"

        /**
         * The height of all rows.
         */
        readonly property real rowsHeight: sourceList.count * sourceMenu.rowHeight

        width: sourceMenu.contentWidth
        height: Math.min(sourceList.rowsHeight, Math.max(0, sourceMenu.maximumContentHeight - menuHeader.height - rescanSection.height))
        interactive: sourceList.rowsHeight > sourceList.height
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        delegate: AbstractInteractiveControl {
            id: sourceRow
            required property int index
            required property string mediaSourceName
            required property string mediaSourceIconUrl
            required property bool active

            objectName: "sourceRow" + sourceRow.index
            width: sourceList.width
            height: sourceMenu.rowHeight
            onClicked: {
                sourceMenu.close();
                if (!sourceRow.active) {
                    sourceMenu.picked(sourceRow.index);
                }
            }

            Rectangle {
                id: rowBackground
                objectName: "sourceRowBackground" + sourceRow.index
                anchors.fill: sourceRow
                color: Theme.colors.secondaryContainer
                visible: sourceRow.active
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
                highlighted: sourceRow.active
                monochrome: sourceRow.mediaSourceIconUrl === ""
                iconSource: sourceRow.mediaSourceIconUrl !== "" ? sourceRow.mediaSourceIconUrl : "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
            }

            StyledText {
                id: sourceName
                objectName: "sourceName" + sourceRow.index
                anchors.left: sourceBadge.right
                anchors.leftMargin: 12
                anchors.right: checkmark.left
                anchors.rightMargin: 12
                anchors.verticalCenter: sourceRow.verticalCenter
                text: sourceRow.mediaSourceName
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.bodyLarge
                maximumLineCount: 1
            }

            Icon {
                id: checkmark
                objectName: "sourceCheckmark" + sourceRow.index
                anchors.right: sourceRow.right
                anchors.rightMargin: 16
                anchors.verticalCenter: sourceRow.verticalCenter
                width: 24
                height: 24
                source: "qrc:/qt/qml/Blabby/Shell/icons/material/check.svg"
                color: Theme.colors.secondary
                visible: sourceRow.active
            }
        }
    }

    Item {
        id: rescanSection
        width: sourceMenu.contentWidth
        height: rescanEntry.y + rescanEntry.height

        Rectangle {
            id: rescanDivider
            objectName: "rescanDivider"
            y: 8
            width: rescanSection.width
            height: 1
            color: Theme.colors.outlineVariant
        }

        AbstractInteractiveControl {
            id: rescanEntry
            objectName: "rescanEntry"
            y: rescanDivider.y + rescanDivider.height + 8
            width: rescanSection.width
            height: 56
            enabled: !sourceMenu.scanning
            onClicked: sourceMenu.rescanRequested()

            Rectangle {
                id: rescanStateLayer
                anchors.fill: rescanEntry
                color: Theme.colors.colorOnSurface
                opacity: rescanEntry.pressed ? Theme.stateLayer.pressedOpacity : rescanEntry.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            Icon {
                id: rescanIcon
                objectName: "rescanIcon"
                anchors.left: rescanEntry.left
                anchors.leftMargin: 16
                anchors.verticalCenter: rescanEntry.verticalCenter
                width: 24
                height: 24
                source: "qrc:/qt/qml/Blabby/Shell/icons/material/refresh.svg"
                color: Theme.colors.colorOnSurfaceVariant
                visible: !sourceMenu.scanning
            }

            BusyIndicator {
                id: rescanBusyIndicator
                objectName: "rescanBusyIndicator"
                anchors.fill: rescanIcon
                color: Theme.colors.colorOnSurfaceVariant
                running: sourceMenu.scanning
            }

            StyledText {
                id: rescanLabel
                objectName: "rescanLabel"
                anchors.left: rescanIcon.right
                anchors.leftMargin: 16
                anchors.right: rescanEntry.right
                anchors.rightMargin: 16
                anchors.verticalCenter: rescanEntry.verticalCenter
                text: qsTr("Rescan network")
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.bodyLarge
                maximumLineCount: 1
            }
        }
    }
}
