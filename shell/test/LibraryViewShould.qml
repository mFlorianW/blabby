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
            mediaItemArtworkUrl: ""
            mediaItemSecondaryText: ""
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "A song with a really very long title that does not fit on the tile"
            mediaItemArtworkUrl: ""
            mediaItemSecondaryText: "Ben Klock featuring a really very long list of artists that does not fit"
            mediaItemType: ItemType.Playable
        }
        ListElement {
            mediaItemTitle: "Video"
            mediaItemArtworkUrl: "qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"
            mediaItemSecondaryText: ""
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Pictures"
            mediaItemArtworkUrl: "qrc:/qt/qml/Blabby/Shell/icons/material/missing.svg"
            mediaItemSecondaryText: ""
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Playlists"
            mediaItemArtworkUrl: ""
            mediaItemSecondaryText: ""
            mediaItemType: ItemType.Container
        }
        ListElement {
            mediaItemTitle: "Podcasts"
            mediaItemArtworkUrl: ""
            mediaItemSecondaryText: ""
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

        SignalSpy {
            id: backRequestedSpy
            target: libraryView
            signalName: "backRequested"
        }

        function init() {
            libraryView.sources = sources;
            libraryView.items = items;
            libraryView.hasActiveSource = true;
            libraryView.activeSourceName = "NAS";
            libraryView.busy = false;
            libraryView.atRoot = true;
            libraryView.containerTitle = "";
            libraryView.visible = true;
            libraryViewTest.child("sourceMenu").close();
            libraryViewTest.child("toast").hide();
            if (sources.count > 2) {
                sources.remove(2, sources.count - 2);
            }
            sourcePickedSpy.clear();
            itemActivatedSpy.clear();
            backRequestedSpy.clear();
            // The actions of the header are laid out on the next polish, clicks before would miss them.
            libraryViewTest.waitForItemPolished(libraryViewTest.child("sourcePill").parent);
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
         * Opens the Source menu with the Source pill and gives it.
         */
        function openMenu() {
            libraryViewTest.mouseClick(libraryViewTest.child("sourcePill"));
            const menu = libraryViewTest.child("sourceMenu");
            libraryViewTest.tryCompare(menu, "opened", true);
            libraryViewTest.waitForRendering(libraryView);
            return menu;
        }

        /**
         * Gives the child of the Source menu with the objectName, null when it doesn't exist.
         */
        function menuChild(objectName) {
            return libraryViewTest.findChild(libraryViewTest.child("sourceMenu"), objectName);
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
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, false);
        }

        /**
         * Tests that the Source pill shows the name of the Active Source with a chevron.
         */
        function test_show_the_name_of_the_active_source_on_the_source_pill() {
            const pill = libraryViewTest.child("sourcePill");
            libraryViewTest.compare(pill.visible, true);
            libraryViewTest.compare(pill.text, "NAS");
            libraryViewTest.compare(pill.showsChevron, true);
            libraryViewTest.compare(pill.checked, false);
        }

        /**
         * The Sources with which the Library has no Active Source.
         */
        function test_show_choose_a_source_on_the_source_pill_without_an_active_source_data() {
            return [
                {
                    tag: "with Sources",
                    sources: sources
                },
                {
                    tag: "without Sources",
                    sources: noSources
                }
            ];
        }

        /**
         * Tests that the Source pill is always shown and reads "Choose a Source" without an Active Source.
         */
        function test_show_choose_a_source_on_the_source_pill_without_an_active_source(data) {
            libraryView.sources = data.sources;
            libraryView.hasActiveSource = false;
            libraryView.activeSourceName = "";
            libraryView.items = noItems;
            const pill = libraryViewTest.child("sourcePill");
            libraryViewTest.compare(pill.visible, true);
            libraryViewTest.compare(pill.text, "Choose a Source");
        }

        /**
         * Tests that the Source menu is closed at start and there is neither a Source picker dialog nor a scrim.
         */
        function test_be_closed_at_start_without_a_dialog() {
            const menu = libraryViewTest.child("sourceMenu");
            libraryViewTest.compare(menu.opened, false);
            libraryViewTest.compare(libraryViewTest.menuChild("sourceMenuPanel").visible, false);
            libraryViewTest.compare(libraryViewTest.findChild(libraryView, "scrim"), null);
        }

        /**
         * Tests that the Source pill opens the menu right below it, aligned to its right edge, and looks checked with a
         * turned chevron meanwhile.
         */
        function test_open_the_source_menu_below_the_source_pill() {
            libraryViewTest.openMenu();
            const pill = libraryViewTest.child("sourcePill");
            const panel = libraryViewTest.menuChild("sourceMenuPanel");
            libraryViewTest.compare(panel.visible, true);
            const pillBottomRight = pill.mapToItem(libraryView, pill.width, pill.height);
            const panelTopRight = panel.mapToItem(libraryView, panel.width, 0);
            libraryViewTest.compare(panelTopRight.x, pillBottomRight.x);
            libraryViewTest.compare(panelTopRight.y, pillBottomRight.y + 8);
            libraryViewTest.compare(panel.width, 380);
            libraryViewTest.compare(pill.checked, true);
            libraryViewTest.tryCompare(libraryViewTest.findChild(pill, "dropDownIcon"), "rotation", 180);
        }

        /**
         * Tests that the button of the "Choose a Source" empty state opens the Source menu.
         */
        function test_open_the_source_menu_with_the_choose_a_source_button() {
            libraryView.hasActiveSource = false;
            libraryView.activeSourceName = "";
            const action = libraryViewTest.findChild(libraryViewTest.child("emptyState"), "action");
            libraryViewTest.verify(action);
            libraryViewTest.waitForItemPolished(action.parent);
            libraryViewTest.mouseClick(action);
            libraryViewTest.tryCompare(libraryViewTest.child("sourceMenu"), "opened", true);
            libraryViewTest.compare(libraryViewTest.child("sourcePill").checked, true);
        }

        /**
         * Tests that the Source menu has the "Choose a Source" header and a 64 px row per Source with its icon in a
         * round 40 px badge and its name elided on one line.
         */
        function test_list_every_source_in_the_source_menu() {
            libraryViewTest.openMenu();
            libraryViewTest.compare(libraryViewTest.menuChild("sourceMenuHeader").text, "Choose a Source");
            const row = libraryViewTest.menuChild("sourceRow0");
            libraryViewTest.verify(row);
            libraryViewTest.compare(row.height, 64);
            const badge = libraryViewTest.menuChild("sourceBadge0");
            libraryViewTest.compare(badge.width, 40);
            libraryViewTest.compare(badge.height, 40);
            libraryViewTest.compare(badge.radius, 20);
            libraryViewTest.compare(badge.iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"));
            const name = libraryViewTest.menuChild("sourceName0");
            libraryViewTest.compare(name.text, "NAS");
            libraryViewTest.compare(name.elide, Text.ElideRight);
            libraryViewTest.compare(name.maximumLineCount, 1);
            libraryViewTest.compare(libraryViewTest.menuChild("sourceName1").text, "Living Room PC");
        }

        /**
         * Tests that a Source without an icon shows the generic "dns" icon in its badge.
         */
        function test_show_the_generic_icon_for_a_source_without_an_icon() {
            sources.append({
                "mediaSourceName": "USB Stick",
                "mediaSourceIconUrl": ""
            });
            libraryViewTest.openMenu();
            libraryViewTest.tryVerify(() => libraryViewTest.menuChild("sourceBadge2") !== null);
            libraryViewTest.compare(libraryViewTest.menuChild("sourceBadge2").iconSource, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg"));
        }

        /**
         * Tests that Sources added or removed while the Source menu is open appear in it or vanish from it.
         */
        function test_follow_sources_added_or_removed_while_open() {
            libraryViewTest.openMenu();
            sources.append({
                "mediaSourceName": "Kitchen Tablet",
                "mediaSourceIconUrl": ""
            });
            libraryViewTest.tryVerify(() => libraryViewTest.menuChild("sourceName2") !== null);
            libraryViewTest.compare(libraryViewTest.menuChild("sourceName2").text, "Kitchen Tablet");
            sources.remove(2);
            libraryViewTest.tryVerify(() => libraryViewTest.menuChild("sourceName2") === null);
        }

        /**
         * Tests that picking a Source emits sourcePicked with its index and closes the menu.
         */
        function test_emit_sourcePicked_and_close_the_menu_when_a_source_is_picked() {
            const menu = libraryViewTest.openMenu();
            const row = libraryViewTest.menuChild("sourceRow1");
            libraryViewTest.verify(row);
            libraryViewTest.mouseClick(row);
            libraryViewTest.compare(sourcePickedSpy.count, 1);
            libraryViewTest.compare(sourcePickedSpy.signalArguments[0][0], 1);
            libraryViewTest.compare(menu.opened, false);
            libraryViewTest.compare(libraryViewTest.child("sourcePill").checked, false);
        }

        /**
         * Tests that tapping the Source pill again closes the menu.
         */
        function test_close_the_source_menu_with_the_source_pill() {
            const menu = libraryViewTest.openMenu();
            libraryViewTest.mouseClick(libraryViewTest.child("sourcePill"));
            libraryViewTest.compare(menu.opened, false);
            libraryViewTest.compare(sourcePickedSpy.count, 0);
        }

        /**
         * Tests that a tap anywhere else in the Library closes the menu without reaching the tile beneath, and that
         * only the Library is covered, so the navigation rail beside it stays usable.
         */
        function test_close_the_source_menu_with_a_tap_elsewhere_without_reaching_the_tile_beneath() {
            const menu = libraryViewTest.openMenu();
            const tapCatcher = libraryViewTest.menuChild("tapCatcher");
            const catcherTopLeft = tapCatcher.mapToItem(libraryView, 0, 0);
            libraryViewTest.compare(catcherTopLeft.x, 0);
            libraryViewTest.compare(catcherTopLeft.y, 0);
            libraryViewTest.compare(tapCatcher.width, libraryView.width);
            libraryViewTest.compare(tapCatcher.height, libraryView.height);
            libraryViewTest.mouseClick(libraryViewTest.tile(0));
            libraryViewTest.compare(menu.opened, false);
            libraryViewTest.compare(itemActivatedSpy.count, 0);
            libraryViewTest.compare(sourcePickedSpy.count, 0);
        }

        /**
         * Tests that the tiles beneath the open Source menu don't react to the hovering mouse.
         */
        function test_not_hover_the_tiles_beneath_the_open_source_menu() {
            libraryViewTest.openMenu();
            const tile = libraryViewTest.tile(0);
            libraryViewTest.mouseMove(tile, tile.width / 2, tile.height / 2);
            libraryViewTest.compare(tile.hovered, false);
        }

        /**
         * Tests that the Source menu is closed when the Library view becomes invisible, e.g. on another screen.
         */
        function test_close_the_source_menu_when_the_library_becomes_invisible() {
            const menu = libraryViewTest.openMenu();
            libraryView.visible = false;
            libraryViewTest.compare(menu.opened, false);
            libraryView.visible = true;
            libraryViewTest.compare(menu.opened, false);
            libraryViewTest.compare(libraryViewTest.menuChild("sourceMenuPanel").visible, false);
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
         * Tests that a tile with artwork shows the artwork instead of the placeholder glyph.
         */
        function test_show_the_artwork_instead_of_the_placeholder() {
            const tile = libraryViewTest.tile(2);
            const artwork = libraryViewTest.findChild(tile, "artwork");
            libraryViewTest.verify(artwork);
            libraryViewTest.compare(tile.artworkUrl, Qt.url("qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg"));
            libraryViewTest.tryCompare(artwork, "visible", true);
            libraryViewTest.compare(libraryViewTest.findChild(tile, "glyph").visible, false);
            libraryViewTest.compare(artwork.width, artwork.height);
            libraryViewTest.compare(artwork.width, tile.width);
        }

        /**
         * Tests that a tile without artwork, or with artwork that can't be loaded, shows the placeholder glyph.
         */
        function test_show_the_placeholder_without_artwork() {
            for (const index of [0, 3]) {
                const tile = libraryViewTest.tile(index);
                libraryViewTest.tryCompare(libraryViewTest.findChild(tile, "glyph"), "visible", true);
                libraryViewTest.compare(libraryViewTest.findChild(tile, "artwork").visible, false);
            }
        }

        /**
         * Tests that a tile shows the secondary text below the title only when there is one.
         */
        function test_show_the_secondary_text_only_when_present() {
            const withArtist = libraryViewTest.findChild(libraryViewTest.tile(1), "secondaryText");
            libraryViewTest.verify(withArtist);
            libraryViewTest.compare(withArtist.visible, true);
            libraryViewTest.compare(withArtist.text, "Ben Klock featuring a really very long list of artists that does not fit");
            libraryViewTest.compare(libraryViewTest.findChild(libraryViewTest.tile(0), "secondaryText").visible, false);
        }

        /**
         * Tests that a too long secondary text is elided instead of overflowing the tile.
         */
        function test_elide_a_too_long_secondary_text() {
            const tile = libraryViewTest.tile(1);
            const secondaryText = libraryViewTest.findChild(tile, "secondaryText");
            libraryViewTest.compare(secondaryText.elide, Text.ElideRight);
            libraryViewTest.verify(secondaryText.truncated);
            libraryViewTest.verify(secondaryText.width <= tile.width);
        }

        /**
         * Tests that a tile fits into its grid cell, also with the secondary text.
         */
        function test_fit_the_tile_with_the_secondary_text_into_its_cell() {
            const grid = libraryViewTest.child("itemGrid");
            libraryViewTest.verify(libraryViewTest.tile(1).height <= grid.cellHeight - libraryView.rowSpacing);
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

        /**
         * Tests that tapping the tile of a Playable emits itemActivated with its index, so that it plays.
         */
        function test_emit_itemActivated_with_the_index_of_a_tapped_playable() {
            libraryViewTest.mouseClick(libraryViewTest.tile(1));
            libraryViewTest.compare(itemActivatedSpy.count, 1);
            libraryViewTest.compare(itemActivatedSpy.signalArguments[0][0], 1);
        }

        /**
         * Tests that the root Container shows neither the back button nor a Container title.
         */
        function test_show_neither_back_button_nor_title_at_the_root() {
            libraryViewTest.compare(libraryViewTest.child("backButton").visible, false);
            libraryViewTest.compare(libraryViewTest.child("containerTitle").visible, false);
        }

        /**
         * Tests that below the root the back button and the title of the current Container are shown.
         */
        function test_show_back_button_and_title_below_the_root() {
            libraryView.atRoot = false;
            libraryView.containerTitle = "Albums";
            const backButton = libraryViewTest.child("backButton");
            libraryViewTest.compare(backButton.visible, true);
            libraryViewTest.verify(backButton.width >= 44);
            libraryViewTest.verify(backButton.height >= 44);
            const title = libraryViewTest.child("containerTitle");
            libraryViewTest.compare(title.visible, true);
            libraryViewTest.compare(title.text, "Albums");
        }

        /**
         * Tests that neither the back button nor a title is shown without an Active Source.
         */
        function test_show_neither_back_button_nor_title_without_an_active_source() {
            libraryView.atRoot = false;
            libraryView.containerTitle = "Albums";
            libraryView.hasActiveSource = false;
            libraryViewTest.compare(libraryViewTest.child("backButton").visible, false);
            libraryViewTest.compare(libraryViewTest.child("containerTitle").visible, false);
        }

        /**
         * Tests that tapping the back button emits backRequested.
         */
        function test_emit_backRequested_when_the_back_button_is_tapped() {
            libraryView.atRoot = false;
            const backButton = libraryViewTest.child("backButton");
            // The header lays out the back button beside the Source pill on the next polish.
            libraryViewTest.verify(libraryViewTest.waitForPolish(backButton.parent));
            libraryViewTest.mouseClick(backButton);
            libraryViewTest.compare(backRequestedSpy.count, 1);
        }

        /**
         * Tests that a busy indicator replaces the grid while a Container opens.
         */
        function test_show_a_busy_indicator_instead_of_the_grid_while_busy() {
            const indicator = libraryViewTest.child("containerBusyIndicator");
            libraryViewTest.compare(indicator.visible, false);

            libraryView.busy = true;
            libraryViewTest.compare(indicator.visible, true);
            libraryViewTest.compare(indicator.running, true);
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, false);
            libraryViewTest.compare(libraryViewTest.child("emptyState").visible, false);
        }

        /**
         * Tests that the busy indicator is shown instead of "Nothing in here" while an empty Container is left.
         */
        function test_show_the_busy_indicator_instead_of_the_empty_state_while_busy() {
            libraryView.items = noItems;
            libraryView.busy = true;
            libraryViewTest.compare(libraryViewTest.child("containerBusyIndicator").visible, true);
            libraryViewTest.compare(libraryViewTest.child("emptyState").visible, false);
        }

        /**
         * Tests that an empty Container shows "Nothing in here" without an action.
         */
        function test_show_nothing_in_here_for_an_empty_container() {
            libraryView.items = noItems;
            const emptyState = libraryViewTest.child("emptyState");
            libraryViewTest.compare(emptyState.visible, true);
            libraryViewTest.compare(emptyState.title, "Nothing in here");
            libraryViewTest.compare(emptyState.actionText, "");
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, false);
            libraryViewTest.compare(libraryViewTest.child("sourcePill").visible, true);
        }

        /**
         * Tests that a Container that couldn't be opened is reported by a toast, while the current Container stays.
         */
        function test_show_a_toast_when_a_container_could_not_be_opened() {
            libraryView.atRoot = false;
            libraryView.containerTitle = "Albums";
            const toast = libraryViewTest.child("toast");
            libraryViewTest.compare(toast.visible, false);

            libraryView.showContainerOpenFailed("Artists");
            libraryViewTest.tryCompare(toast, "visible", true);
            libraryViewTest.compare(libraryViewTest.findChild(toast, "message").text, "Couldn't open Artists");
            libraryViewTest.compare(libraryViewTest.child("containerTitle").text, "Albums");
            libraryViewTest.compare(libraryViewTest.child("itemGrid").visible, true);
            libraryViewTest.compare(libraryViewTest.tile(0).visible, true);
        }
    }
}
