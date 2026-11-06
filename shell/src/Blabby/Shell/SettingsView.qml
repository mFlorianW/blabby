// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Controls
import Blabby.Theme

/**
 * The screen to configure Blabby.
 * It lists the sections on the left and shows the rows of the picked section on the right, the picked section is
 * highlighted. A row shows a value, a @ref SwitchRow for a switch, or a button for an action of a Provider.
 */
Item {
    id: settingsView

    /**
     * The sections to list, a model with the roles "title" and "iconSource".
     */
    property alias sections: sectionList.model

    /**
     * The index of the picked section.
     */
    property int currentSection: 0

    /**
     * The rows of the picked section, a model with the roles "title", "subtitle", "value", "kind", a value of
     * SettingsRowKind, and "checked".
     */
    property alias rows: rowList.model

    /**
     * This signal is emitted when the user taps the section at index.
     */
    signal sectionPicked(int index)

    /**
     * This signal is emitted when the user taps the switch of the row at index, checked is the value the user asks for.
     */
    signal switchToggled(int index, bool checked)

    /**
     * This signal is emitted when the user taps the button of the action in the row at index.
     */
    signal actionTriggered(int index)

    ScreenHeader {
        id: header
        anchors.top: settingsView.top
        anchors.left: settingsView.left
        anchors.right: settingsView.right
        anchors.margins: 24
        title: qsTr("Settings")
    }

    ListView {
        id: sectionList
        objectName: "sectionList"
        anchors.top: header.bottom
        anchors.topMargin: 16
        anchors.bottom: settingsView.bottom
        anchors.bottomMargin: 24
        anchors.left: header.left
        width: 300
        spacing: 4
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        currentIndex: settingsView.currentSection

        delegate: AbstractInteractiveControl {
            id: section
            required property int index
            required property string title
            required property url iconSource

            /**
             * True when the section is the picked one.
             */
            readonly property bool picked: section.index === settingsView.currentSection

            /**
             * The colour of the icon, the label and the state layer, depending on the picked state.
             */
            readonly property color contentColor: section.picked ? Theme.colors.colorOnSecondaryContainer : Theme.colors.colorOnSurface

            objectName: "section" + section.index
            width: sectionList.width
            height: 72
            onClicked: settingsView.sectionPicked(section.index)

            Rectangle {
                id: background
                anchors.fill: section
                radius: section.height / 2
                color: section.picked ? Theme.colors.secondaryContainer : "transparent"
            }

            Rectangle {
                id: stateLayer
                anchors.fill: background
                radius: background.radius
                color: section.contentColor
                opacity: section.pressed ? Theme.stateLayer.pressedOpacity : section.hovered ? Theme.stateLayer.hoverOpacity : 0
            }

            Icon {
                id: icon
                anchors.left: section.left
                anchors.leftMargin: 16
                anchors.verticalCenter: section.verticalCenter
                width: 24
                height: 24
                source: section.iconSource
                color: section.contentColor
            }

            StyledText {
                id: label
                anchors.left: icon.right
                anchors.leftMargin: 16
                anchors.right: section.right
                anchors.rightMargin: 16
                anchors.verticalCenter: section.verticalCenter
                text: section.title
                color: section.contentColor
                textStyle: Theme.fonts.titleMedium
            }
        }
    }

    Rectangle {
        id: pane
        objectName: "sectionPane"
        anchors.top: sectionList.top
        anchors.bottom: sectionList.bottom
        anchors.left: sectionList.right
        anchors.leftMargin: 24
        anchors.right: header.right
        radius: 28
        color: Theme.colors.surfaceContainerLow

        StyledText {
            id: paneTitle
            objectName: "sectionTitle"
            anchors.top: pane.top
            anchors.topMargin: 20
            anchors.left: pane.left
            anchors.leftMargin: 28
            anchors.right: pane.right
            anchors.rightMargin: 28
            text: sectionList.currentItem ? sectionList.currentItem.title : ""
            color: Theme.colors.colorOnSurface
            textStyle: Theme.fonts.titleLarge
        }

        ListView {
            id: rowList
            objectName: "rowList"
            anchors.top: paneTitle.bottom
            anchors.topMargin: 8
            anchors.bottom: pane.bottom
            anchors.bottomMargin: 16
            anchors.left: pane.left
            anchors.leftMargin: 16
            anchors.right: pane.right
            anchors.rightMargin: 16
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            delegate: Item {
                id: row
                required property int index
                required property string title
                required property string subtitle
                required property string value
                required property int kind
                required property bool checked

                objectName: "row" + row.index
                width: rowList.width
                height: 72

                SwitchRow {
                    id: switchRow
                    objectName: "switchRow"
                    anchors.fill: row
                    visible: row.kind === SettingsRowKind.Switch
                    title: row.title
                    subtitle: row.subtitle
                    checked: row.checked
                    onToggled: checked => settingsView.switchToggled(row.index, checked)
                }

                Item {
                    id: actionRow
                    objectName: "actionRow"
                    anchors.fill: row
                    visible: row.kind === SettingsRowKind.Action

                    Column {
                        id: actionTexts
                        anchors.left: actionRow.left
                        anchors.leftMargin: 12
                        anchors.right: actionButton.left
                        anchors.rightMargin: 16
                        anchors.verticalCenter: actionRow.verticalCenter

                        StyledText {
                            objectName: "title"
                            width: actionTexts.width
                            text: row.title
                            color: Theme.colors.colorOnSurface
                            textStyle: Theme.fonts.bodyLarge
                        }

                        StyledText {
                            objectName: "subtitle"
                            width: actionTexts.width
                            text: row.subtitle
                            color: Theme.colors.colorOnSurfaceVariant
                            textStyle: Theme.fonts.bodyMedium
                            visible: row.subtitle !== ""
                        }
                    }

                    Button {
                        id: actionButton
                        objectName: "actionButton"
                        anchors.right: actionRow.right
                        anchors.rightMargin: 12
                        anchors.verticalCenter: actionRow.verticalCenter
                        // Taller than the default button to be a large enough touch target.
                        height: 48
                        text: row.title
                        onClicked: settingsView.actionTriggered(row.index)
                    }
                }

                Column {
                    id: texts
                    visible: row.kind === SettingsRowKind.Info
                    anchors.left: row.left
                    anchors.leftMargin: 12
                    anchors.right: valueText.left
                    anchors.rightMargin: 16
                    anchors.verticalCenter: row.verticalCenter

                    StyledText {
                        id: titleText
                        width: texts.width
                        text: row.title
                        color: Theme.colors.colorOnSurface
                        textStyle: Theme.fonts.bodyLarge
                    }

                    StyledText {
                        id: subtitleText
                        width: texts.width
                        text: row.subtitle
                        color: Theme.colors.colorOnSurfaceVariant
                        textStyle: Theme.fonts.bodyMedium
                        visible: row.subtitle !== ""
                    }
                }

                StyledText {
                    id: valueText
                    objectName: "valueText"
                    visible: row.kind === SettingsRowKind.Info
                    anchors.right: row.right
                    anchors.rightMargin: 12
                    anchors.verticalCenter: row.verticalCenter
                    text: row.value
                    color: Theme.colors.colorOnSurface
                    textStyle: Theme.fonts.labelLarge
                }
            }
        }
    }
}
