// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Lookup.hpp"
#include <QQuickAsyncImageProvider>

namespace Shell
{

/**
 * The @ref Shell::ArtworkImageProvider serves the looked-up Artwork of albums to QML as
 * "image://artwork/?artist=…&album=…", see `Shell::artworkUrl()`. Without Artwork the image fails to load, so the QML
 * shows its placeholder.
 */
class ArtworkImageProvider final : public QQuickAsyncImageProvider
{
public:
    /**
     * Creates an instance of the @ref Shell::ArtworkImageProvider.
     * @param lookup The lookup of the Artwork, it must outlive the provider and lives in the main thread.
     */
    explicit ArtworkImageProvider(Artwork::Lookup& lookup);
    ~ArtworkImageProvider() override;
    Q_DISABLE_COPY_MOVE(ArtworkImageProvider)

    /**
     * Requests the Artwork of an album.
     * @param id The part of the URL after "image://artwork/", e.g. "?artist=…&album=…".
     * @param requestedSize The size the image is scaled down to, an invalid size keeps the size of the Artwork.
     * @return The response, it finishes asynchronously with the Artwork or with an error without Artwork.
     */
    QQuickImageResponse* requestImageResponse(QString const& id, QSize const& requestedSize) override;

private:
    Artwork::Lookup& mLookup;
};

} // namespace Shell
