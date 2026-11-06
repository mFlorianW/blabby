// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * The menu to pick the Active Source that drops down under the Source pill, headed "Choose a Source" with a row per
 * Source that shows its icon in a round badge and its name. The row of the Active Source is highlighted, its badge is
 * drawn in the highlighted colours and it carries a checkmark. Sources that appear or vanish while the menu is open show
 * up in or vanish from it. The tap catcher, the panel, the anchoring and opening and closing come from @ref MenuShell.
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
     * This signal is emitted when the user picks the Source at index that isn't the Active Source.
     */
    signal picked(int index)

    /**
     * The height of a row of the list.
     */
    readonly property real rowHeight: 64

    panel.objectName: "sourceMenuPanel"
    panelWidth: 380

    StyledText {
        id: header
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

    ListView {
        id: sourceList
        objectName: "sourceList"
        width: sourceMenu.contentWidth
        height: sourceList.count * sourceMenu.rowHeight
        interactive: false

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
}
