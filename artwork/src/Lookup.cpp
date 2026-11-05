// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Lookup.hpp"
#include "private/DiskCache.hpp"
#include "private/MusicBrainzService.hpp"
#include <QStandardPaths>
#include <QTimer>

namespace Artwork
{

namespace
{

constexpr auto maximumArtworkSize = 500;

QString albumKey(QString const& artist, QString const& album)
{
    return artist.trimmed().toLower() + QLatin1Char('\n') + album.trimmed().toLower();
}

/**
 * Finishes the request later, so that the caller can connect to it first.
 */
void finishLater(std::shared_ptr<Request> const& request, QImage image)
{
    QTimer::singleShot(0, request.get(), [request = request.get(), image = std::move(image)] {
        request->finish(image);
    });
}

} // namespace

/**
 * A lookup of an album that is in progress and the requests waiting for it.
 */
struct OnlineLookup::PendingLookup
{
    std::shared_ptr<Request> lookup;
    QList<std::weak_ptr<Request>> requests;
};

Lookup::~Lookup() = default;

OnlineLookup::OnlineLookup()
    : OnlineLookup{std::make_unique<NetworkHttpBackend>(),
                   std::make_unique<SteadyScheduler>(),
                   QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/artwork")}
{
}

OnlineLookup::OnlineLookup(std::unique_ptr<HttpBackend> backend,
                           std::unique_ptr<Scheduler> scheduler,
                           QString cacheDirectory)
    : mBackend{std::move(backend)}
    , mScheduler{std::move(scheduler)}
    , mService{std::make_unique<MusicBrainzService>(*mBackend, *mScheduler)}
    , mCache{std::make_unique<DiskCache>(std::move(cacheDirectory))}
{
}

OnlineLookup::~OnlineLookup() = default;

std::shared_ptr<Request> OnlineLookup::requestArtwork(QString const& artist, QString const& album)
{
    auto request = std::make_shared<Request>();
    auto const key = albumKey(artist, album);

    if (mAlbumsWithoutArtwork.contains(key)) {
        finishLater(request, QImage{});
        return request;
    }
    if (auto const uncached = mUncachedArtwork.constFind(key); uncached != mUncachedArtwork.cend()) {
        finishLater(request, uncached.value());
        return request;
    }

    // The cache is read in another thread, so that decoding the Artwork doesn't block the caller.
    (void)mCache->read(key).then(
        this,
        [this, weakRequest = std::weak_ptr{request}, key, artist, album](QImage const& cached) {
            auto const request = weakRequest.lock();
            if (request == nullptr or request->isCancelled()) {
                return;
            }
            if (not cached.isNull()) {
                request->finish(cached);
            } else if (mAlbumsWithoutArtwork.contains(key)) {
                // Another request of the album found no Artwork meanwhile.
                request->finish(QImage{});
            } else {
                lookUp(key, artist, album, request);
            }
        });
    return request;
}

void OnlineLookup::lookUp(QString const& key,
                          QString const& artist,
                          QString const& album,
                          std::shared_ptr<Request> const& request)
{
    auto& pending = mPendingLookups[key];
    if (pending == nullptr) {
        pending = std::make_shared<PendingLookup>();
        pending->lookup = mService->lookUp(artist, album);
        (void)connect(pending->lookup.get(), &Request::finished, this, [this, key] {
            onLookupFinished(key);
        });
    }
    pending->requests.append(request);
    (void)connect(request.get(), &Request::cancelled, this, [this, key] {
        onRequestCancelled(key);
    });
}

void OnlineLookup::onLookupFinished(QString const& key)
{
    auto const pending = mPendingLookups.take(key);
    if (pending == nullptr) {
        return;
    }

    auto image = pending->lookup->image();
    if (image.isNull()) {
        mAlbumsWithoutArtwork.insert(key);
    } else {
        if (image.width() > maximumArtworkSize or image.height() > maximumArtworkSize) {
            image = image.scaled(maximumArtworkSize, maximumArtworkSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        if (not mCache->write(key, image)) {
            mUncachedArtwork.insert(key, image);
        }
    }
    for (auto const& weakRequest : std::as_const(pending->requests)) {
        if (auto const request = weakRequest.lock(); request != nullptr) {
            request->finish(image);
        }
    }
}

void OnlineLookup::onRequestCancelled(QString const& key)
{
    auto const pending = mPendingLookups.value(key);
    if (pending == nullptr) {
        return;
    }

    pending->requests.removeIf([](auto const& weakRequest) {
        auto const request = weakRequest.lock();
        return request == nullptr or request->isCancelled();
    });
    if (pending->requests.isEmpty()) {
        mPendingLookups.remove(key);
        pending->lookup->cancel();
    }
}

} // namespace Artwork
