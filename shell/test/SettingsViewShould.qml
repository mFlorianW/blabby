// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Shell

Item {
    id: root
    // The window size of 1280 x 720 px minus the navigation rail.
    width: 1200
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
        id: aboutRows
        ListElement {
            title: "Blabby version"
            subtitle: ""
            value: "0.1.0"
        }
        ListElement {
            title: "Qt version"
            subtitle: ""
            value: "6.5.3"
        }
        ListElement {
            title: "MediaServerProvider"
            subtitle: "Provider"
            value: "0.2.0"
        }
    }

    SettingsView {
        id: settingsView
        anchors.fill: root
        sections: sections
        rows: aboutRows
        onSectionPicked: index => settingsView.currentSection = index
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

        function init() {
            settingsView.currentSection = 1;
            sectionPickedSpy.clear();
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
    }
}
