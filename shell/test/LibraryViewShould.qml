// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Objects
import Blabby.Shell

Item {
    id: root
    // The window width of 1280 px minus the navigation rail.
    width: 1200
    height: 640

    ListModel {
        id: sources
        ListElement {
            mediaSourceName: "NAS"
            mediaSourceIconUrl: "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
        }
        ListElement {
            mediaSourceName: "Living Room PC"
            mediaSourceIconUrl: "qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"
        }
    }

    ListModel {
        id: noSources
    }

    ListModel {
        id: items
        ListElement {
            mediaItemTitle: "Music"
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "A song with a really very long title that does not fit on the tile"
            mediaItemType: ItemType.Playable
        }
        ListElement {
            mediaItemTitle: "Video"
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Pictures"
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Playlists"
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Podcasts"
            mediaItemType: ItemType.Container
        }
    }

    ListModel {
        id: noItems
    }

    LibraryView {
        id: libraryView
        anchors.fill: parent
    }

    TestCase {
        id: libraryViewTest
        name: "LibraryViewShould"
        when: windowShown

        SignalSpy {
            id: sourcePickedSpy
            target: libraryView
            signalName: "sourcePicked"
        }

        SignalSpy {
            id: itemActivatedSpy
            target: libraryView
            signalName: "itemActivated"
        }

        function init() {
            libraryView.sources = sources;
            libraryView.items = items;
            libraryView.hasActiveSource = true;
            libraryView.activeSourceName = "NAS";
            libraryViewTest.child("sourcePicker").visible = false;
            if (sources.count > 2) {
                sources.remove(2, sources.count - 2);
            }
            sourcePickedSpy.clear();
            itemActivatedSpy.clear();
        }

        /**
         * Gives the child of the view with the objectName and fails the test when it doesn't exist.
         */
        function child(objectName) {
            const item = libraryViewTest.findChild(libraryView, objectName);
            libraryViewTest.verify(item, objectName);
            return item;
        }

        /**
         * Gives the tile of the Item at index and fails the test when it doesn't exist.
         */
        function tile(index) {
            return libraryViewTest.child("itemTile" + index);
        }

        /**
         * Tests that the "No Sources found" empty state is shown without Sources.
         */
        function test_show_no_sources_found_without_sources() {
            libraryView.sources = noSources;
            libraryView.hasActiveSource = false;
            libraryView.activeSourceName = "";
            libraryView.items = noItems;
            const emptyState = libraryViewTest.child("emptyState");
            libraryViewTest.compare(emptyState.visible, true);
            libraryViewTest.compare(emptyState.title, "No Sources found");
            libraryViewTest.compare(emptyState.actionText, "");
            libraryViewTest.compare(libraryViewTest.child("sourcePill").visible, false);
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, false);
        }

        /**
         * Tests that "Choose a Source" with a button is shown with Sources but without an Active Source.
         */
        function test_show_choose_a_source_without_an_active_source() {
            libraryView.hasActiveSource = false;
            libraryView.activeSourceName = "";
            libraryView.items = noItems;
            const emptyState = libraryViewTest.child("emptyState");
            libraryViewTest.compare(emptyState.visible, true);
            libraryViewTest.compare(emptyState.title, "Choose a Source");
            libraryViewTest.compare(libraryViewTest.child("sourcePill").visible, false);
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, false);
        }

        /**
         * Tests that the button of the "Choose a Source" empty state opens the Source picker.
         */
        function test_open_the_source_picker_with_the_choose_a_source_button() {
            libraryView.hasActiveSource = false;
            libraryView.activeSourceName = "";
            const action = libraryViewTest.findChild(libraryViewTest.child("emptyState"), "action");
            libraryViewTest.verify(action);
            libraryViewTest.mouseClick(action);
            libraryViewTest.tryCompare(libraryViewTest.child("sourcePicker"), "visible", true);
        }

        /**
         * Tests that the Source pill shows the name of the Active Source and opens the Source picker.
         */
        function test_open_the_source_picker_with_the_source_pill() {
            const pill = libraryViewTest.child("sourcePill");
            libraryViewTest.compare(pill.visible, true);
            libraryViewTest.compare(pill.text, "NAS");
            libraryViewTest.compare(libraryViewTest.child("emptyState").visible, false);
            libraryViewTest.mouseClick(pill);
            libraryViewTest.tryCompare(libraryViewTest.child("sourcePicker"), "visible", true);
        }

        /**
         * Tests that the Source picker lists every Source with name and icon, also the ones appearing while it is open.
         */
        function test_list_every_source_in_the_source_picker() {
            libraryViewTest.mouseClick(libraryViewTest.child("sourcePill"));
            const picker = libraryViewTest.child("sourcePicker");
            libraryViewTest.tryCompare(picker, "visible", true);
            libraryViewTest.compare(libraryViewTest.findChild(picker, "sourceName0").text, "NAS");
            libraryViewTest.compare(libraryViewTest.findChild(picker, "sourceIcon0").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"));
            libraryViewTest.compare(libraryViewTest.findChild(picker, "sourceName1").text, "Living Room PC");

            sources.append({
                "mediaSourceName": "Kitchen Tablet",
                "mediaSourceIconUrl": ""
            });
            libraryViewTest.tryVerify(() => libraryViewTest.findChild(picker, "sourceName2") !== null);
            libraryViewTest.compare(libraryViewTest.findChild(picker, "sourceName2").text, "Kitchen Tablet");
        }

        /**
         * Tests that picking a Source emits sourcePicked with its index and closes the picker.
         */
        function test_emit_sourcePicked_and_close_the_picker_when_a_source_is_picked() {
            libraryViewTest.mouseClick(libraryViewTest.child("sourcePill"));
            const picker = libraryViewTest.child("sourcePicker");
            libraryViewTest.tryCompare(picker, "visible", true);
            const row = libraryViewTest.findChild(picker, "sourceRow1");
            libraryViewTest.verify(row);
            libraryViewTest.verify(row.height >= 44);
            libraryViewTest.mouseClick(row);
            libraryViewTest.compare(sourcePickedSpy.count, 1);
            libraryViewTest.compare(sourcePickedSpy.signalArguments[0][0], 1);
            libraryViewTest.tryCompare(picker, "visible", false);
        }

        /**
         * Tests that the tiles show the title and a folder glyph for a Container and a note glyph for a Playable.
         */
        function test_show_the_title_and_the_glyph_of_the_item_type() {
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, true);
            const container = libraryViewTest.tile(0);
            libraryViewTest.compare(libraryViewTest.findChild(container, "title").text, "Music");
            libraryViewTest.compare(libraryViewTest.findChild(container, "glyph").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/folder.svg"));
            const playable = libraryViewTest.tile(1);
            libraryViewTest.compare(libraryViewTest.findChild(playable, "glyph").source, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/music_note.svg"));
        }

        /**
         * Tests that a too long title is elided instead of overflowing the tile.
         */
        function test_elide_a_too_long_title() {
            const tile = libraryViewTest.tile(1);
            const title = libraryViewTest.findChild(tile, "title");
            libraryViewTest.compare(title.elide, Text.ElideRight);
            libraryViewTest.verify(title.truncated);
            libraryViewTest.verify(title.width <= tile.width);
        }

        /**
         * Tests that the grid shows five columns at the 1280 px display and the tiles are large touch targets.
         */
        function test_show_five_columns_at_the_1280_px_display() {
            libraryViewTest.waitForRendering(libraryView);
            const firstRowY = libraryViewTest.tile(0).mapToItem(libraryView, 0, 0).y;
            const tilesInFirstRow = [0, 1, 2, 3, 4, 5].filter(index => libraryViewTest.tile(index).mapToItem(libraryView, 0, 0).y === firstRowY);
            libraryViewTest.compare(tilesInFirstRow.length, 5);
            libraryViewTest.verify(libraryViewTest.tile(0).width >= 44);
            libraryViewTest.verify(libraryViewTest.tile(0).height >= 44);
        }

        /**
         * Tests that tapping a tile emits itemActivated with the index of the Item.
         */
        function test_emit_itemActivated_with_the_index_of_the_tapped_tile() {
            libraryViewTest.mouseClick(libraryViewTest.tile(2));
            libraryViewTest.compare(itemActivatedSpy.count, 1);
            libraryViewTest.compare(itemActivatedSpy.signalArguments[0][0], 2);
        }
    }
}
