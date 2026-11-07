// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma Singleton
import QtQml

/**
 * Sizes the rows of a list or grid so that the last visible row is cut off by about a third.
 * A list that ends exactly on a row edge looks finished, so a touch user doesn't expect more to scroll to.
 * The rows start at the top of the viewport and are rowHeight high with spacing between two of them.
 */
QtObject {
    /**
     * The part of the last visible row that is shown, the cut off part.
     */
    readonly property real cutOffShown: 2 / 3

    /**
     * Gives the part of the row cut off by the bottom edge of the viewport that the viewport shows, 0 when the edge
     * cuts no row because it lies on a row edge or in the spacing.
     */
    function shownFraction(viewportHeight: real, rowHeight: real, spacing: real): real {
        const remainder = viewportHeight % (rowHeight + spacing);
        return remainder < rowHeight ? remainder / rowHeight : 0;
    }

    /**
     * Gives how far the part of the cut off row that the viewport shows is from the cut off part, Infinity when the
     * viewport doesn't show a whole row above the cut off one.
     */
    function cutOffDistance(viewportHeight: real, rowHeight: real, spacing: real): real {
        if (viewportHeight < rowHeight + spacing) {
            return Infinity;
        }
        return Math.abs(shownFraction(viewportHeight, rowHeight, spacing) - cutOffShown);
    }

    /**
     * Gives the row height closest to the preferred one with which the viewport shows the cut off part of the last
     * visible row, at least one whole row above it. When the viewport is too low for a whole row of the preferred
     * height and the cut off part of the next one, it gives the preferred height, so that the rows don't shrink below
     * their content.
     */
    function rowHeight(viewportHeight: real, preferredRowHeight: real, spacing: real): real {
        if (viewportHeight < (1 + cutOffShown) * preferredRowHeight + spacing) {
            return preferredRowHeight;
        }
        // The number of whole rows above the cut off one with the preferred row height.
        const wholeRows = (viewportHeight - cutOffShown * preferredRowHeight) / (preferredRowHeight + spacing);
        const heightFor = rows => (viewportHeight - rows * spacing) / (rows + cutOffShown);
        const fewer = heightFor(Math.max(1, Math.floor(wholeRows)));
        const more = heightFor(Math.max(1, Math.ceil(wholeRows)));
        return Math.abs(fewer - preferredRowHeight) <= Math.abs(more - preferredRowHeight) ? fewer : more;
    }

    /**
     * Gives the spacing from minimumSpacing to maximumSpacing with which the viewport shows the part of the cut off row
     * closest to the cut off part, at least one whole row above it. Of the spacings that show the same part
     * it gives the one closest to the preferred one, and the preferred one when no whole row fits.
     */
    function spacing(viewportHeight: real, rowHeight: real, preferredSpacing: real, minimumSpacing: real, maximumSpacing: real): real {
        if (viewportHeight < rowHeight) {
            return preferredSpacing;
        }
        let best = preferredSpacing;
        let bestDistance = Infinity;
        const mostWholeRows = Math.floor((viewportHeight - cutOffShown * rowHeight) / (rowHeight + minimumSpacing));
        for (let wholeRows = 1; wholeRows <= mostWholeRows + 1; ++wholeRows) {
            const exact = (viewportHeight - (wholeRows + cutOffShown) * rowHeight) / wholeRows;
            const candidate = Math.min(maximumSpacing, Math.max(minimumSpacing, exact));
            const distance = cutOffDistance(viewportHeight, rowHeight, candidate);
            // Equal distances differ by rounding errors only.
            const tolerance = 1e-9;
            const closer = distance < bestDistance - tolerance;
            const asClose = Math.abs(distance - bestDistance) <= tolerance;
            if (closer || (asClose && Math.abs(candidate - preferredSpacing) < Math.abs(best - preferredSpacing))) {
                best = candidate;
                bestDistance = distance;
            }
        }
        return best;
    }

    /**
     * Gives the content height capped to the maximum height, for rows without spacing. A content height above the
     * maximum height is capped to the highest height up to it that shows the cut off part of the last visible row, or
     * to the maximum height when that is below the cut off part of a single row.
     */
    function cappedHeight(contentHeight: real, maximumHeight: real, rowHeight: real): real {
        if (contentHeight <= maximumHeight) {
            return contentHeight;
        }
        const wholeRows = Math.floor((maximumHeight - cutOffShown * rowHeight) / rowHeight);
        return wholeRows < 0 ? maximumHeight : (wholeRows + cutOffShown) * rowHeight;
    }
}
