// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * The dialog to pick the Active Source, listing every Source with its name and icon.
 * Sources that appear while the dialog is open show up in the list. Cancel closes it without picking a Source.
 */
Dialog {
    id: sourcePicker

    /**
     * The Sources to list, a model with the roles "mediaSourceName" and "mediaSourceIconUrl".
     */
    property alias model: sourceList.model

    /**
     * The number of Sources in the list.
     */
    readonly property alias count: sourceList.count

    /**
     * This signal is emitted when the user picks the Source at index.
     */
    signal picked(int index)

    /**
     * The height of a row of the list.
     */
    readonly property real rowHeight: 56

    /**
     * The most rows that are shown without scrolling.
     */
    readonly property int visibleRows: 5

    title: qsTr("Choose a Source")

    ListView {
        id: sourceList
        objectName: "sourceList"
        width: sourcePicker.contentWidth
        height: Math.min(sourceList.count, sourcePicker.visibleRows) * sourcePicker.rowHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        delegate: AbstractInteractiveControl {
            id: sourceRow
            required property int index
            required property string mediaSourceName
            required property string mediaSourceIconUrl

            objectName: "sourceRow" + sourceRow.index
            width: sourceList.width
            height: sourcePicker.rowHeight
            onClicked: sourcePicker.picked(sourceRow.index)

            Rectangle {
                id: stateLayer
                anchors.fill: sourceRow
                color: Theme.colors.colorOnSurface
                opacity: sourceRow.pressed ? Theme.stateLayer.pressedOpacity : sourceRow.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            Image {
                id: sourceIcon
                objectName: "sourceIcon" + sourceRow.index
                anchors.left: sourceRow.left
                anchors.leftMargin: 8
                anchors.verticalCenter: sourceRow.verticalCenter
                width: 24
                height: 24
                sourceSize.width: 24
                sourceSize.height: 24
                fillMode: Image.PreserveAspectFit
                source: sourceRow.mediaSourceIconUrl
            }

            StyledText {
                id: sourceName
                objectName: "sourceName" + sourceRow.index
                anchors.left: sourceIcon.right
                anchors.leftMargin: 16
                anchors.right: sourceRow.right
                anchors.rightMargin: 8
                anchors.verticalCenter: sourceRow.verticalCenter
                text: sourceRow.mediaSourceName
                color: Theme.colors.colorOnSurface
                textStyle: Theme.fonts.bodyLarge
            }
        }
    }
}
