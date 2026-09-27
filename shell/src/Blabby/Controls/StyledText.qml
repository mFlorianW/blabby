// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import Blabby.Theme

/**
 * A text rendered in a style of the Theme type scale, e.g. Theme.fonts.bodyMedium.
 */
Text {
    id: styledText

    /**
     * The style of the Theme type scale the text is rendered in.
     */
    property QtObject textStyle: Theme.fonts.bodyMedium

    elide: Text.ElideRight
    font.family: styledText.textStyle.family
    font.pixelSize: styledText.textStyle.size
    font.weight: styledText.textStyle.weight
    font.letterSpacing: styledText.textStyle.letterSpacing
    lineHeight: styledText.textStyle.lineHeight
    lineHeightMode: Text.FixedHeight
}
