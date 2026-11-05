// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ArtworkImageProvider.hpp"
#include <QObject>
#include <memory>

namespace Shell
{

/**
 * A @ref Artwork::Lookup whose requests are finished by the test.
 */
class LookupDouble final : public Artwork::Lookup
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(LookupDouble)
public:
    LookupDouble();
    ~LookupDouble() override;

    std::shared_ptr<Artwork::Request> requestArtwork(QString const& artist, QString const& album) override;

    QString lastArtist;
    QString lastAlbum;
    std::shared_ptr<Artwork::Request> lastRequest;
};

class ArtworkImageProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ArtworkImageProviderShould() override;
    Q_DISABLE_COPY_MOVE(ArtworkImageProviderShould)

private:
    std::unique_ptr<LookupDouble> mLookup;
    std::unique_ptr<ArtworkImageProvider> mProvider;

private Q_SLOTS:
    void init();
    void request_the_artwork_of_the_album_in_the_url();
    void deliver_the_artwork();
    void fail_without_artwork();
    void forward_the_cancellation_to_the_lookup();
};

} // namespace Shell
