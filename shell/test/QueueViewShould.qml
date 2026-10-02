// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls
import Blabby.Shell

Item {
    id: root
    // The window width of 1280 px minus the navigation rail.
    width: 1200
    height: 1280

    ListModel {
        id: entries
        ListElement {
            title: "Salt on the Rail"
            artist: "The Quiet Ferries"
            album: "Low Tide Sessions"
            artworkUrl: "http://art/1.jpg"
            duration: 238000
            hasDuration: true
            current: false
        }
        ListElement {
            title: "Harbour Lights"
            artist: ""
            album: ""
            artworkUrl: ""
            duration: 0
            hasDuration: false
            current: true
        }
        ListElement {
            title: "Signal Fires"
            artist: "Aurel Vance"
            album: "Northern Static"
            artworkUrl: ""
            duration: 302000
            hasDuration: true
            current: false
        }
    }

    ListModel {
        id: noEntries
    }

    QueueView {
        id: queueView
        width: root.width
        height: 640
        entries: entries
    }

    // Placed below the view with entries to not overlap it.
    QueueView {
        id: emptyView
        y: queueView.height
        width: root.width
        height: queueView.height
        entries: noEntries
    }

    TestCase {
        id: queueViewTest
        name: "QueueViewShould"
        when: windowShown

        SignalSpy {
            id: playRequestedSpy
            target: queueView
            signalName: "playRequested"
        }

        SignalSpy {
            id: chooseRendererRequestedSpy
            target: queueView
            signalName: "chooseRendererRequested"
        }

        SignalSpy {
            id: removeRequestedSpy
            target: queueView
            signalName: "removeRequested"
        }

        SignalSpy {
            id: moveRequestedSpy
            target: queueView
            signalName: "moveRequested"
        }

        SignalSpy {
            id: clearRequestedSpy
            target: queueView
            signalName: "clearRequested"
        }

        SignalSpy {
            id: undoRequestedSpy
            target: queueView
            signalName: "undoRequested"
        }

        SignalSpy {
            id: browseLibraryRequestedSpy
            target: emptyView
            signalName: "browseLibraryRequested"
        }

        function init() {
            queueView.hasActiveRenderer = true;
            queueView.currentEntryPlaying = true;
            queueView.entryCount = 3;
            queueView.totalDuration = 540000;
            queueView.hasTotalDuration = true;
            queueView.totalDurationPartial = true;
            queueViewTest.findChild(queueView, "toast").hide();
            playRequestedSpy.clear();
            chooseRendererRequestedSpy.clear();
            removeRequestedSpy.clear();
            moveRequestedSpy.clear();
            clearRequestedSpy.clear();
            undoRequestedSpy.clear();
            browseLibraryRequestedSpy.clear();
        }

        /**
         * Gives the row of the entry at the index.
         */
        function row(index: int): Item {
            return queueViewTest.findChild(queueView, "entryRow" + index);
        }

        /**
         * Drags the row at the index by its handle vertically by dy. The pointer is moved in the coordinates of the
         * view, the handle moves along with the row.
         */
        function dragByHandle(index: int, dy: real) {
            const handle = queueViewTest.findChild(queueViewTest.row(index), "dragHandle");
            const start = handle.mapToItem(queueView, handle.width / 2, handle.height / 2);
            queueViewTest.mousePress(queueView, start.x, start.y);
            for (let step = 1; step <= 4; ++step) {
                queueViewTest.mouseMove(queueView, start.x, start.y + dy * step / 4, -1, Qt.LeftButton);
            }
            queueViewTest.mouseRelease(queueView, start.x, start.y + dy);
        }

        /**
         * Gives the text of the child with the object name.
         */
        function textOf(parent: Item, objectName: string): string {
            return queueViewTest.findChild(parent, objectName).text;
        }

        /**
         * Tests that each row shows the number, title, artist, album and duration of its entry.
         */
        function test_show_the_entries() {
            const first = queueViewTest.row(0);
            queueViewTest.verify(first !== null);
            queueViewTest.compare(queueViewTest.textOf(first, "number"), "1");
            queueViewTest.compare(queueViewTest.textOf(first, "title"), "Salt on the Rail");
            queueViewTest.compare(queueViewTest.textOf(first, "artist"), "The Quiet Ferries");
            queueViewTest.compare(queueViewTest.textOf(first, "album"), "Low Tide Sessions");
            queueViewTest.compare(queueViewTest.textOf(first, "duration"), "3:58");
            queueViewTest.compare(queueViewTest.findChild(first, "artwork").source.toString(), "http://art/1.jpg");
            queueViewTest.compare(queueViewTest.textOf(queueViewTest.row(2), "number"), "3");
        }

        /**
         * Tests that the unknown parts of an entry are left out.
         */
        function test_leave_out_unknown_parts() {
            const second = queueViewTest.row(1);
            queueViewTest.compare(queueViewTest.findChild(second, "artist").visible, false);
            queueViewTest.compare(queueViewTest.findChild(second, "album").visible, false);
            queueViewTest.compare(queueViewTest.findChild(second, "duration").visible, false);
        }

        /**
         * Tests that the Current Entry is highlighted with the equaliser glyph instead of its number.
         */
        function test_highlight_the_current_entry() {
            const current = queueViewTest.row(1);
            queueViewTest.compare(current.current, true);
            queueViewTest.compare(queueViewTest.findChild(current, "equalizer").visible, true);
            queueViewTest.compare(queueViewTest.findChild(current, "number").visible, false);
            const other = queueViewTest.row(0);
            queueViewTest.compare(queueViewTest.findChild(other, "equalizer").visible, false);
            queueViewTest.compare(queueViewTest.findChild(other, "number").visible, true);
            queueViewTest.verify(!Qt.colorEqual(queueViewTest.findChild(current, "background").color, queueViewTest.findChild(other, "background").color));
        }

        /**
         * Tests that the equaliser glyph only moves while the Active Renderer plays the Current Entry for the Queue.
         */
        function test_animate_the_equalizer_only_while_the_current_entry_plays() {
            const equalizer = queueViewTest.findChild(queueViewTest.row(1), "equalizer");
            queueViewTest.compare(equalizer.running, true);
            queueView.currentEntryPlaying = false;
            queueViewTest.compare(equalizer.running, false);
        }

        function test_summarize_the_queue_data() {
            return [
                {
                    tag: "all durations known",
                    entryCount: 8,
                    totalDuration: 34 * 60000,
                    hasTotalDuration: true,
                    totalDurationPartial: false,
                    summary: "8 tracks · 34 min"
                },
                {
                    tag: "some durations unknown",
                    entryCount: 8,
                    totalDuration: 30 * 60000 + 20000,
                    hasTotalDuration: true,
                    totalDurationPartial: true,
                    summary: "8 tracks · ≥ 30 min"
                },
                {
                    tag: "some durations unknown, rounded down",
                    entryCount: 8,
                    totalDuration: 30 * 60000 + 40000,
                    hasTotalDuration: true,
                    totalDurationPartial: true,
                    summary: "8 tracks · ≥ 30 min"
                },
                {
                    tag: "less than a minute known",
                    entryCount: 2,
                    totalDuration: 20000,
                    hasTotalDuration: true,
                    totalDurationPartial: true,
                    summary: "2 tracks"
                },
                {
                    tag: "no duration known",
                    entryCount: 2,
                    totalDuration: 0,
                    hasTotalDuration: false,
                    totalDurationPartial: true,
                    summary: "2 tracks"
                },
                {
                    tag: "one short track",
                    entryCount: 1,
                    totalDuration: 20000,
                    hasTotalDuration: true,
                    totalDurationPartial: false,
                    summary: "1 track · 1 min"
                }
            ];
        }

        /**
         * Tests that the header summarizes the number of entries and the total duration.
         */
        function test_summarize_the_queue(data) {
            queueView.entryCount = data.entryCount;
            queueView.totalDuration = data.totalDuration;
            queueView.hasTotalDuration = data.hasTotalDuration;
            queueView.totalDurationPartial = data.totalDurationPartial;
            queueViewTest.compare(queueViewTest.findChild(queueView, "header").subtitle, data.summary);
        }

        /**
         * Tests that tapping a row asks to play its entry.
         */
        function test_ask_to_play_a_tapped_entry() {
            queueViewTest.mouseClick(queueViewTest.row(2));
            queueViewTest.compare(playRequestedSpy.count, 1);
            queueViewTest.compare(playRequestedSpy.signalArguments[0][0], 2);
            queueViewTest.compare(queueViewTest.findChild(queueView, "toast").shown, false);
        }

        /**
         * Tests that tapping a row without an Active Renderer asks to play it and tells to choose a Renderer.
         */
        function test_tell_to_choose_a_renderer_when_tapping_an_entry_without_an_active_renderer() {
            queueView.hasActiveRenderer = false;
            queueViewTest.mouseClick(queueViewTest.row(0));
            queueViewTest.compare(playRequestedSpy.count, 1);
            const toast = queueViewTest.findChild(queueView, "toast");
            queueViewTest.compare(toast.shown, true);
            queueViewTest.compare(queueViewTest.findChild(toast, "message").text, "Choose a Renderer to play the Queue");
            queueViewTest.tryCompare(toast, "visible", true);

            queueViewTest.mouseClick(queueViewTest.findChild(toast, "action"));

            queueViewTest.compare(chooseRendererRequestedSpy.count, 1);
        }

        /**
         * Tests that an empty Queue shows an empty state that leads to the Library.
         */
        function test_show_an_empty_state_that_leads_to_the_library() {
            const emptyState = queueViewTest.findChild(emptyView, "emptyState");
            queueViewTest.compare(emptyState.visible, true);
            queueViewTest.compare(emptyState.title, "The Queue is empty");
            queueViewTest.compare(queueViewTest.findChild(emptyView, "entryList").visible, false);
            queueViewTest.compare(queueViewTest.findChild(queueView, "emptyState").visible, false);

            queueViewTest.mouseClick(queueViewTest.findChild(emptyState, "action"));

            queueViewTest.compare(browseLibraryRequestedSpy.count, 1);
        }

        /**
         * Tests that swiping a row away asks to remove its entry and offers to undo it.
         */
        function test_ask_to_remove_a_swiped_entry_and_offer_undo() {
            const first = queueViewTest.row(0);
            queueViewTest.mouseDrag(first, first.width / 2, first.height / 2, -first.width / 2, 0);

            queueViewTest.compare(removeRequestedSpy.count, 1);
            queueViewTest.compare(removeRequestedSpy.signalArguments[0][0], 0);
            queueViewTest.compare(playRequestedSpy.count, 0);
            const toast = queueViewTest.findChild(queueView, "toast");
            queueViewTest.compare(toast.shown, true);
            queueViewTest.compare(queueViewTest.findChild(toast, "message").text, "Removed “Salt on the Rail”");
            queueViewTest.compare(queueViewTest.findChild(toast, "action").text, "Undo");
            queueViewTest.tryCompare(toast, "visible", true);

            queueViewTest.mouseClick(queueViewTest.findChild(toast, "action"));

            queueViewTest.compare(undoRequestedSpy.count, 1);
            queueViewTest.compare(chooseRendererRequestedSpy.count, 0);
        }

        /**
         * Tests that a short swipe neither removes the entry nor plays it, the row slides back.
         */
        function test_keep_an_entry_on_a_short_swipe() {
            const first = queueViewTest.row(0);
            queueViewTest.mouseDrag(first, first.width / 2, first.height / 2, -40, 0);

            queueViewTest.compare(removeRequestedSpy.count, 0);
            queueViewTest.compare(playRequestedSpy.count, 0);
            queueViewTest.tryCompare(queueViewTest.findChild(first, "swipeOffset"), "x", 0);
            queueViewTest.compare(first.pressed, false);
        }

        function test_ask_to_move_an_entry_by_its_handle_data() {
            return [
                {
                    tag: "down",
                    row: 0,
                    dy: 2 * 62,
                    to: 2
                },
                {
                    tag: "up",
                    row: 2,
                    dy: -62,
                    to: 1
                },
                {
                    tag: "past the end",
                    row: 1,
                    dy: 5 * 62,
                    to: 2
                }
            ];
        }

        /**
         * Tests that dragging a row by its handle asks to move its entry to where it is dropped.
         */
        function test_ask_to_move_an_entry_by_its_handle(data) {
            queueViewTest.dragByHandle(data.row, data.dy);

            queueViewTest.compare(moveRequestedSpy.count, 1);
            queueViewTest.compare(moveRequestedSpy.signalArguments[0][0], data.row);
            queueViewTest.compare(moveRequestedSpy.signalArguments[0][1], data.to);
            queueViewTest.compare(playRequestedSpy.count, 0);
            queueViewTest.compare(removeRequestedSpy.count, 0);
        }

        /**
         * Tests that a row dropped at its own place isn't moved.
         */
        function test_not_move_an_entry_dropped_at_its_place() {
            queueViewTest.dragByHandle(1, 20);

            queueViewTest.compare(moveRequestedSpy.count, 0);
            queueViewTest.compare(playRequestedSpy.count, 0);
        }

        /**
         * Tests that Clear asks to clear the Queue without a confirmation and offers to undo it.
         */
        function test_ask_to_clear_and_offer_undo() {
            const clearButton = queueViewTest.findChild(queueView, "clearButton");
            queueViewTest.compare(clearButton.visible, true);

            queueViewTest.mouseClick(clearButton);

            queueViewTest.compare(clearRequestedSpy.count, 1);
            const toast = queueViewTest.findChild(queueView, "toast");
            queueViewTest.compare(toast.shown, true);
            queueViewTest.compare(queueViewTest.findChild(toast, "message").text, "Cleared the Queue");
            queueViewTest.tryCompare(toast, "visible", true);

            queueViewTest.mouseClick(queueViewTest.findChild(toast, "action"));

            queueViewTest.compare(undoRequestedSpy.count, 1);
        }

        /**
         * Tests that Clear is hidden while the Queue is empty.
         */
        function test_hide_clear_when_the_queue_is_empty() {
            queueViewTest.compare(queueViewTest.findChild(emptyView, "clearButton").visible, false);
        }
    }
}
