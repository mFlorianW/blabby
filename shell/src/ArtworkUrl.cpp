// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ArtworkUrl.hpp"
#include <QUrl>

namespace Shell
{

QString artworkUrl(QString const& suppliedUrl, QString const& artist, QString const& album)
{
    if (not suppliedUrl.isEmpty()) {
        return suppliedUrl;
    }
    if (album.isEmpty()) {
        return {};
    }
    return QStringLiteral("image://artwork/?artist=%1&album=%2")
        .arg(QString::fromLatin1(QUrl::toPercentEncoding(artist)), QString::fromLatin1(QUrl::toPercentEncoding(album)));
}

QString artworkUrl(Multimedia::Item const& item)
{
    auto const& artist = item.albumArtist().isEmpty() ? item.secondaryText() : item.albumArtist();
    return artworkUrl(item.artworkUrl(), artist, item.album());
}

QString artworkUrl(Multimedia::CurrentTrack const& track)
{
    auto const& artist = track.albumArtist.isEmpty() ? track.artist : track.albumArtist;
    return artworkUrl(track.artworkUrl, artist, track.album);
}

} // namespace Shell
