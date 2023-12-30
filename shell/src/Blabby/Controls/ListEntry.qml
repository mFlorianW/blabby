// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
import QtQuick
import Blabby.Controls 1.0
import Blabby.Shell 1.0
import Blabby.Theme 1.0

AbstractListEntry{
    id: listEntry
    implicitHeight: 48

    Rectangle{
        id: stateLayer
        anchors.fill:parent
        color: "transparent"
    }

    Image {
        id: entryIcon
        source: listEntry.iconUrl
        width: 24
        height: 24
        anchors.left: listEntry.left
        anchors.verticalCenter: listEntry.verticalCenter
        anchors.leftMargin: 20
    }

    Text {
        id: entryTitle
        anchors.left: entryIcon.right
        anchors.leftMargin: 20
        anchors.right: trailingItemContainer.left
        anchors.top: listEntry.top
        anchors.bottom: listEntry.bottom
        verticalAlignment:  Text.AlignVCenter
        text: listEntry.title
        font.family: Theme.fonts.bodyLarge.family
        font.pixelSize: Theme.fonts.bodyLarge.size
        color: Theme.fonts.bodyLarge.color
        elide: Text.ElideRight
    }

    Item{
        id: trailingItemContainer
        anchors.right: listEntry.right
        anchors.rightMargin: 22
        anchors.top: listEntry.top
        anchors.bottom: listEntry.bottom
        children: listEntry.trailingElement
        clip: true

        onChildrenChanged: () => {
            for (var i = 0; i < trailingItemContainer.children.length; ++i) {
                trailingItemContainer.children[i].anchors.centerIn = trailingItemContainer
                trailingItemContainer.width = trailingItemContainer.children[i].width
            }
        }
    }

    states:[
        State{
            name: "hovered"
            when: listEntry.hovered === true
            PropertyChanges{
                target: stateLayer
                color: Theme.stateColors.onSurfaceVariant.opacity08
                opacity: 0.08
            }
        }
    ]
}
