// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtTest
import Blabby.Controls

TestCase {
    name: "CutOffRowsShould"

    /**
     * The viewports and rows with the part of the cut off row that is shown.
     */
    function test_tell_how_much_of_the_cut_off_row_is_shown_data() {
        return [
            {
                tag: "a row cut in half",
                viewportHeight: 250,
                rowHeight: 100,
                spacing: 0,
                shown: 0.5
            },
            {
                tag: "a row cut below the spacing",
                viewportHeight: 270,
                rowHeight: 100,
                spacing: 10,
                shown: 0.5
            },
            {
                tag: "the rows ending on a row edge",
                viewportHeight: 300,
                rowHeight: 100,
                spacing: 0,
                shown: 0
            },
            {
                tag: "the rows ending in the spacing",
                viewportHeight: 215,
                rowHeight: 100,
                spacing: 10,
                shown: 0
            }
        ];
    }

    /**
     * Tests that the shown part of the cut off row is told, and that rows ending on the edge of the viewport or in the
     * spacing cut off no row.
     */
    function test_tell_how_much_of_the_cut_off_row_is_shown(data) {
        compare(CutOffRows.shownFraction(data.viewportHeight, data.rowHeight, data.spacing), data.shown);
    }

    /**
     * The viewports and rows with the distance of their cut off row to the one cut off by a third.
     */
    function test_tell_how_far_the_cut_off_row_is_from_one_cut_off_by_a_third_data() {
        return [
            {
                tag: "a row cut in half",
                viewportHeight: 250,
                rowHeight: 100,
                spacing: 0,
                distance: 2 / 3 - 0.5
            },
            {
                tag: "the rows ending on a row edge",
                viewportHeight: 300,
                rowHeight: 100,
                spacing: 0,
                distance: 2 / 3
            },
            {
                tag: "the only row cut off",
                viewportHeight: 70,
                rowHeight: 100,
                spacing: 0,
                distance: Infinity
            },
            {
                tag: "the second row cut off in the spacing",
                viewportHeight: 105,
                rowHeight: 100,
                spacing: 10,
                distance: Infinity
            }
        ];
    }

    /**
     * Tests that the distance to a row cut off by a third is told, and that a cut off row without a whole row above it
     * is the farthest.
     */
    function test_tell_how_far_the_cut_off_row_is_from_one_cut_off_by_a_third(data) {
        const distance = CutOffRows.cutOffDistance(data.viewportHeight, data.rowHeight, data.spacing);
        if (data.distance === Infinity) {
            compare(distance, Infinity);
        } else {
            fuzzyCompare(distance, data.distance, 0.001);
        }
    }

    /**
     * The viewports and preferred rows to size the rows for.
     */
    function test_give_a_row_height_that_cuts_off_the_last_visible_row_by_a_third_data() {
        return [
            {
                tag: "without spacing",
                viewportHeight: 500,
                rowHeight: 60,
                spacing: 0
            },
            {
                tag: "with spacing",
                viewportHeight: 500,
                rowHeight: 60,
                spacing: 4
            },
            {
                tag: "for rows that fit exactly",
                viewportHeight: 480,
                rowHeight: 60,
                spacing: 0
            },
            {
                tag: "for less than two rows",
                viewportHeight: 190,
                rowHeight: 100,
                spacing: 16
            }
        ];
    }

    /**
     * Tests that the rows are sized so that the last visible one is cut off by a third.
     */
    function test_give_a_row_height_that_cuts_off_the_last_visible_row_by_a_third(data) {
        const rowHeight = CutOffRows.rowHeight(data.viewportHeight, data.rowHeight, data.spacing);

        fuzzyCompare(CutOffRows.shownFraction(data.viewportHeight, rowHeight, data.spacing), CutOffRows.cutOffShown, 0.001);
    }

    /**
     * Tests that of the row heights that cut off the last visible row the one closest to the preferred one is given.
     */
    function test_keep_the_row_height_close_to_the_preferred_one() {
        // 500 px fit 8.33 rows of 60 px, 8.67 rows of 57.7 px are closer than 7.67 rows of 65.2 px.
        fuzzyCompare(CutOffRows.rowHeight(500, 60, 0), 500 / (8 + CutOffRows.cutOffShown), 0.001);
        // 530 px fit 8.83 rows of 60 px, 8.67 rows of 61.2 px are closer than 9.67 rows of 54.8 px.
        fuzzyCompare(CutOffRows.rowHeight(530, 60, 0), 530 / (8 + CutOffRows.cutOffShown), 0.001);
    }

    /**
     * The viewports too low for a whole row of the preferred height and the cut off part of the next one.
     */
    function test_keep_the_preferred_row_height_when_a_row_and_its_cut_off_part_do_not_fit_data() {
        return [
            {
                tag: "without a viewport",
                viewportHeight: 0
            },
            {
                tag: "a viewport lower than a row",
                viewportHeight: 30
            },
            {
                tag: "a viewport lower than a row and its cut off part",
                viewportHeight: 100
            }
        ];
    }

    /**
     * Tests that the rows keep their preferred height in a viewport too low to cut off a row below a whole one, so
     * that they don't shrink below their content.
     */
    function test_keep_the_preferred_row_height_when_a_row_and_its_cut_off_part_do_not_fit(data) {
        compare(CutOffRows.rowHeight(data.viewportHeight, 60, 4), 60);
    }

    /**
     * Tests that a height above the maximum height is capped so that the last visible row is cut off by a third.
     */
    function test_cap_a_height_so_that_the_last_visible_row_is_cut_off_by_a_third() {
        const height = CutOffRows.cappedHeight(800, 500, 64);

        compare(height, (7 + CutOffRows.cutOffShown) * 64);
        fuzzyCompare(CutOffRows.shownFraction(height, 64, 0), CutOffRows.cutOffShown, 0.001);
    }

    /**
     * Tests that a height up to the maximum height isn't capped, its rows all fit.
     */
    function test_not_cap_a_height_up_to_the_maximum_height() {
        compare(CutOffRows.cappedHeight(192, 200, 64), 192);
        compare(CutOffRows.cappedHeight(200, 200, 64), 200);
    }

    /**
     * Tests that a height is capped to the maximum height when that is below the cut off part of a single row.
     */
    function test_cap_a_height_to_the_maximum_height_when_it_is_below_the_cut_off_part_of_a_row() {
        compare(CutOffRows.cappedHeight(800, 30, 64), 30);
    }

    /**
     * Tests that the spacing is picked from the range so that the last visible row is cut off by a third.
     */
    function test_give_a_spacing_in_the_range_that_cuts_off_the_last_visible_row_by_a_third() {
        const spacing = CutOffRows.spacing(480, 100, 24, 16, 40);

        verify(spacing >= 16 && spacing <= 40, spacing);
        fuzzyCompare(CutOffRows.shownFraction(480, 100, spacing), CutOffRows.cutOffShown, 0.001);
    }

    /**
     * Tests that of the spacings that cut off the last visible row by a third the one closest to the preferred one is
     * given.
     */
    function test_give_the_spacing_closest_to_the_preferred_one_of_those_that_cut_off_the_last_visible_row() {
        // 13 whole rows above the cut off one need 24.4 px, 14 rows 19 px and 12 rows 30.6 px.
        fuzzyCompare(CutOffRows.spacing(1000, 50, 24, 16, 40), (1000 - (13 + CutOffRows.cutOffShown) * 50) / 13, 0.001);
    }

    /**
     * Tests that the spacing stays in the range when no spacing in it cuts off the last visible row by a third.
     */
    function test_keep_the_spacing_in_the_range_when_no_spacing_in_it_cuts_off_by_a_third() {
        // 3 whole rows need 44.4 px, which shows 0.8 of the cut off row with 40 px, 4 whole rows need 8.3 px.
        compare(CutOffRows.spacing(500, 100, 24, 16, 40), 40);
    }

    /**
     * Tests that the preferred spacing is kept when not even a single row fits.
     */
    function test_keep_the_preferred_spacing_when_no_row_fits() {
        compare(CutOffRows.spacing(50, 100, 24, 16, 40), 24);
    }
}
