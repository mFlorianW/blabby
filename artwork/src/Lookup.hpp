// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpBackend.hpp"
#include "Request.hpp"
#include "Scheduler.hpp"
#include "blabbyartwork_export.h"
#include <QHash>
#include <QObject>
#include <QSet>
#include <memory>

namespace Artwork
{

class DiskCache;
class Service;

/**
 * The @ref Artwork::Lookup gives the Artwork of an album that is looked up elsewhere, e.g. on the internet.
 */
class BLABBYARTWORK_EXPORT Lookup : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Lookup)
public:
    ~Lookup() override;

    /**
     * Requests the Artwork of an album.
     * @param artist The artist of the album, empty when unknown.
     * @param album The title of the album.
     * @return The request, it finishes asynchronously with the Artwork or with "no Artwork".
     */
    virtual std::shared_ptr<Request> requestArtwork(QString const& artist, QString const& album) = 0;

protected:
    Lookup() = default;
};

/**
 * The @ref Artwork::OnlineLookup looks the Artwork of an album up on the internet, on MusicBrainz and the Cover Art
 * Archive. Found Artwork is scaled down to at most 500 px and kept in a cache directory, so it survives restarts.
 * An album without Artwork, also one whose lookup failed, is only remembered for the lifetime of the instance: it's
 * not looked up again while the instance lives, but by the next one.
 */
class BLABBYARTWORK_EXPORT OnlineLookup final : public Lookup
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(OnlineLookup)
public:
    /**
     * Creates an instance that looks up with Qt's network access and the system's monotonic clock, and caches in the
     * "artwork" directory of the application's cache location.
     */
    OnlineLookup();

    /**
     * Creates an instance with the given dependencies.
     * @param backend The backend that sends the HTTP requests.
     * @param scheduler The scheduler for the rate limit of the online service.
     * @param cacheDirectory The directory that keeps the found Artwork.
     */
    OnlineLookup(std::unique_ptr<HttpBackend> backend, std::unique_ptr<Scheduler> scheduler, QString cacheDirectory);

    ~OnlineLookup() override;

    std::shared_ptr<Request> requestArtwork(QString const& artist, QString const& album) override;

private:
    struct PendingLookup;
    void lookUp(QString const& key,
                QString const& artist,
                QString const& album,
                std::shared_ptr<Request> const& request);
    void onLookupFinished(QString const& key);
    void onRequestCancelled(QString const& key);

    std::unique_ptr<HttpBackend> mBackend;
    std::unique_ptr<Scheduler> mScheduler;
    std::unique_ptr<Service> mService;
    std::unique_ptr<DiskCache> mCache;
    QSet<QString> mAlbumsWithoutArtwork;
    // Found Artwork that couldn't be written to the cache, so that it's not looked up again.
    QHash<QString, QImage> mUncachedArtwork;
    QHash<QString, std::shared_ptr<PendingLookup>> mPendingLookups;
};

} // namespace Artwork
