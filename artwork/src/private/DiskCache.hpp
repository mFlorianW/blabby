// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QFuture>
#include <QImage>
#include <QString>

namespace Artwork
{

/**
 * The @ref Artwork::DiskCache keeps found Artwork in a directory, so that it survives restarts. It reads in the global
 * thread pool, so that decoding the Artwork doesn't block its user.
 */
class DiskCache final
{
public:
    /**
     * Creates an instance of the @ref Artwork::DiskCache.
     * @param directory The directory of the cache, it's created when the first Artwork is written.
     */
    explicit DiskCache(QString directory);

    /**
     * Gives whether there is cached Artwork of the album, without reading it.
     * @param key The key of the album.
     * @return True when there is cached Artwork.
     */
    bool contains(QString const& key) const;

    /**
     * Reads the cached Artwork.
     * @param key The key of the album.
     * @return The Artwork once it's read, a null image when there is none in the cache.
     */
    QFuture<QImage> read(QString const& key) const;

    /**
     * Writes the Artwork to the cache, replacing the one of the album.
     * @param key The key of the album.
     * @param image The Artwork.
     * @return True when the Artwork is written.
     */
    bool write(QString const& key, QImage const& image) const;

private:
    QString filePath(QString const& key) const;

    QString mDirectory;
};

} // namespace Artwork
