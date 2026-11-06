// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Shell

Item {
    id: root
    // The window size of 1280 x 720 px minus the navigation rail.
    width: 1184
    height: 720

    ListModel {
        id: sections
        ListElement {
            title: "Renderers"
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
        }
        ListElement {
            title: "About"
            iconSource: "qrc:/qt/qml/Blabby/Shell/icons/material/info.svg"
        }
    }

    ListModel {
        id: rendererRows
        ListElement {
            title: "Remember last Renderer"
            subtitle: "Make the Renderer you last picked active again when it is Online"
            value: ""
            kind: SettingsRowKind.Switch
            checked: true
        }
    }

    ListModel {
        id: aboutRows
        ListElement {
            title: "Blabby version"
            subtitle: ""
            value: "0.1.0"
            kind: SettingsRowKind.Info
            checked: false
        }
        ListElement {
            title: "Qt version"
            subtitle: ""
            value: "6.5.3"
            kind: SettingsRowKind.Info
            checked: false
        }
        ListElement {
            title: "MediaServerProvider"
            subtitle: "Provider"
            value: "0.2.0"
            kind: SettingsRowKind.Info
            checked: false
        }
    }

    SettingsView {
        id: settingsView
        anchors.fill: root
        sections: sections
        rows: settingsView.currentSection === 0 ? rendererRows : aboutRows
        onSectionPicked: index => settingsView.currentSection = index
        onSwitchToggled: (index, checked) => rendererRows.setProperty(index, "checked", checked)
    }

    TestCase {
        id: settingsViewTest
        name: "SettingsViewShould"
        when: windowShown

        SignalSpy {
            id: sectionPickedSpy
            target: settingsView
            signalName: "sectionPicked"
        }

        SignalSpy {
            id: switchToggledSpy
            target: settingsView
            signalName: "switchToggled"
        }

        function init() {
            settingsView.currentSection = 1;
            rendererRows.setProperty(0, "checked", true);
            sectionPickedSpy.clear();
            switchToggledSpy.clear();
        }

        /**
         * Gives the child of the view with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = settingsViewTest.findChild(settingsView, objectName);
            settingsViewTest.verify(item, objectName);
            return item;
        }

        /**
         * Tests that the sections are listed in the order of the model, one below the other.
         */
        function test_list_the_sections_in_order() {
            const renderers = settingsViewTest.child("section0");
            const about = settingsViewTest.child("section1");
            settingsViewTest.compare(renderers.title, "Renderers");
            settingsViewTest.compare(about.title, "About");
            settingsViewTest.verify(renderers.mapToItem(settingsView, 0, 0).y < about.mapToItem(settingsView, 0, 0).y);
        }

        /**
         * Tests that only the picked section is highlighted and its title is shown above its rows.
         */
        function test_highlight_the_picked_section() {
            settingsViewTest.compare(settingsViewTest.child("section0").picked, false);
            settingsViewTest.compare(settingsViewTest.child("section1").picked, true);
            settingsViewTest.compare(settingsViewTest.child("sectionTitle").text, "About");
        }

        /**
         * Tests that the rows are shown next to the section list.
         */
        function test_show_the_rows_next_to_the_section_list() {
            const sectionList = settingsViewTest.child("sectionList");
            const pane = settingsViewTest.child("sectionPane");
            settingsViewTest.verify(pane.mapToItem(settingsView, 0, 0).x >= sectionList.mapToItem(settingsView, sectionList.width, 0).x);
            const row = settingsViewTest.child("row0");
            settingsViewTest.verify(row.mapToItem(settingsView, 0, 0).x >= pane.mapToItem(settingsView, 0, 0).x);
        }

        /**
         * Tests that tapping a section emits sectionPicked with its index and highlights it once picked.
         */
        function test_pick_a_tapped_section() {
            settingsViewTest.mouseClick(settingsViewTest.child("section0"));
            settingsViewTest.compare(sectionPickedSpy.count, 1);
            settingsViewTest.compare(sectionPickedSpy.signalArguments[0][0], 0);
            settingsViewTest.compare(settingsViewTest.child("section0").picked, true);
            settingsViewTest.compare(settingsViewTest.child("section1").picked, false);
            settingsViewTest.compare(settingsViewTest.child("sectionTitle").text, "Renderers");
        }

        /**
         * Tests that the About rows show the Blabby version, the Qt version and each Provider with its version.
         */
        function test_show_the_about_rows() {
            settingsViewTest.compare(settingsViewTest.child("rowList").count, 3);
            const expected = [["Blabby version", "", "0.1.0"], ["Qt version", "", "6.5.3"], ["MediaServerProvider", "Provider", "0.2.0"]];
            expected.forEach(([title, subtitle, value], index) => {
                const row = settingsViewTest.child("row" + index);
                settingsViewTest.compare(row.title, title);
                settingsViewTest.compare(row.subtitle, subtitle);
                settingsViewTest.compare(row.value, value);
            });
        }

        /**
         * Tests that the About rows show their value and no switch.
         */
        function test_show_no_switch_in_the_about_rows() {
            const row = settingsViewTest.child("row0");
            settingsViewTest.compare(settingsViewTest.findChild(row, "valueText").visible, true);
            settingsViewTest.compare(settingsViewTest.findChild(row, "switchRow").visible, false);
        }

        /**
         * Tests that the Renderers section shows the "Remember last Renderer" switch with its value.
         */
        function test_show_the_remember_last_renderer_switch_in_the_renderers_section() {
            settingsView.currentSection = 0;
            settingsViewTest.compare(settingsViewTest.child("rowList").count, 1);
            const switchRow = settingsViewTest.findChild(settingsViewTest.child("row0"), "switchRow");
            settingsViewTest.verify(switchRow);
            settingsViewTest.compare(switchRow.visible, true);
            settingsViewTest.compare(switchRow.title, "Remember last Renderer");
            settingsViewTest.compare(switchRow.subtitle, "Make the Renderer you last picked active again when it is Online");
            settingsViewTest.compare(switchRow.checked, true);
            settingsViewTest.compare(settingsViewTest.findChild(settingsViewTest.child("row0"), "valueText").visible, false);
        }

        /**
         * Tests that tapping the switch emits switchToggled with its row and the new value, and shows the stored value.
         */
        function test_toggle_the_tapped_switch() {
            settingsView.currentSection = 0;
            const switchRow = settingsViewTest.findChild(settingsViewTest.child("row0"), "switchRow");
            settingsViewTest.mouseClick(switchRow);
            settingsViewTest.compare(switchToggledSpy.count, 1);
            settingsViewTest.compare(switchToggledSpy.signalArguments[0][0], 0);
            settingsViewTest.compare(switchToggledSpy.signalArguments[0][1], false);
            settingsViewTest.compare(switchRow.checked, false);
            settingsViewTest.mouseClick(switchRow);
            settingsViewTest.compare(switchToggledSpy.count, 2);
            settingsViewTest.compare(switchToggledSpy.signalArguments[1][1], true);
            settingsViewTest.compare(switchRow.checked, true);
        }
    }
}
