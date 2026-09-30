// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Effects
import Blabby.Controls
import Blabby.Theme

/**
 * A rounded artwork, e.g. an album art. Without artwork, or when the artwork can't be loaded, a tinted placeholder
 * carries a glyph instead.
 */
Rectangle {
    id: artwork

    /**
     * The URL of the artwork, empty when there is none.
     */
    property url source

    /**
     * The URL of the SVG glyph of the placeholder.
     */
    property url glyphSource

    /**
     * The colour of the placeholder, the secondary container colour by default.
     */
    property color placeholderColor: Theme.colors.secondaryContainer

    /**
     * True when the artwork is loaded and shown instead of the placeholder glyph.
     */
    readonly property bool hasArtwork: artworkImage.status === Image.Ready

    radius: 16
    color: artwork.placeholderColor

    Icon {
        id: glyph
        objectName: "glyph"
        anchors.centerIn: artwork
        width: artwork.width * 0.4
        height: artwork.height * 0.4
        source: artwork.glyphSource
        color: Theme.colors.colorOnSecondaryContainer
        opacity: 0.5
        visible: !artwork.hasArtwork
    }

    Image {
        id: artworkImage
        anchors.fill: artwork
        source: artwork.source
        sourceSize.width: artwork.width
        sourceSize.height: artwork.height
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        visible: false
    }

    Rectangle {
        id: artworkMask
        anchors.fill: artwork
        radius: artwork.radius
        visible: false
        layer.enabled: true
    }

    // Rounds the corners of the artwork like the placeholder.
    MultiEffect {
        id: roundedArtwork
        objectName: "artwork"
        anchors.fill: artwork
        source: artworkImage
        maskEnabled: true
        maskSource: artworkMask
        visible: artwork.hasArtwork
    }
}
