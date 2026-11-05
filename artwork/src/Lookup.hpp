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
#include <memory>

namespace Artwork
{

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
 * Archive. The outcome of a lookup, found or not, is kept for the lifetime of the instance, so an album is looked up
 * once.
 */
class BLABBYARTWORK_EXPORT OnlineLookup final : public Lookup
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(OnlineLookup)
public:
    /**
     * Creates an instance that looks up with Qt's network access and the system's monotonic clock.
     */
    OnlineLookup();

    /**
     * Creates an instance with the given dependencies.
     * @param backend The backend that sends the HTTP requests.
     * @param scheduler The scheduler for the rate limit of the online service.
     */
    OnlineLookup(std::unique_ptr<HttpBackend> backend, std::unique_ptr<Scheduler> scheduler);

    ~OnlineLookup() override;

    std::shared_ptr<Request> requestArtwork(QString const& artist, QString const& album) override;

private:
    struct PendingLookup;
    void onLookupFinished(QString const& key);
    void onRequestCancelled(QString const& key);

    std::unique_ptr<HttpBackend> mBackend;
    std::unique_ptr<Scheduler> mScheduler;
    std::unique_ptr<Service> mService;
    QHash<QString, QImage> mResults;
    QHash<QString, std::shared_ptr<PendingLookup>> mPendingLookups;
};

} // namespace Artwork
