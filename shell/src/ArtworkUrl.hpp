// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Item.hpp"
#include <QString>

namespace Shell
{

/**
 * Gives the URL of the Artwork to show for an Item. The Artwork supplied by the Source or the Renderer always wins.
 * Without it, an album with a known title gets the URL of its looked-up Artwork, served by the "artwork" image
 * provider as "image://artwork/?artist=…&album=…". Otherwise there is no Artwork and the URL is empty.
 * @param suppliedUrl The URL of the Artwork supplied by the Source or the Renderer, empty when there is none.
 * @param artist The artist of the album, empty when unknown.
 * @param album The title of the album, empty when unknown.
 * @return The URL of the Artwork, empty without Artwork.
 */
QString artworkUrl(QString const& suppliedUrl, QString const& artist, QString const& album);

/**
 * Gives the URL of the Artwork to show for an Item, like @ref artworkUrl(QString const&, QString const&, QString
 * const&). The album is named by its Album Artist, by the artist of the Item without an Album Artist.
 * @param item The Item.
 * @return The URL of the Artwork, empty without Artwork.
 */
QString artworkUrl(Multimedia::Item const& item);

} // namespace Shell
