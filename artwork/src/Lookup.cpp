// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Lookup.hpp"
#include "private/MusicBrainzService.hpp"
#include <QTimer>

namespace Artwork
{

namespace
{

QString albumKey(QString const& artist, QString const& album)
{
    return artist.trimmed().toLower() + QLatin1Char('\n') + album.trimmed().toLower();
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
    : OnlineLookup{std::make_unique<NetworkHttpBackend>(), std::make_unique<SteadyScheduler>()}
{
}

OnlineLookup::OnlineLookup(std::unique_ptr<HttpBackend> backend, std::unique_ptr<Scheduler> scheduler)
    : mBackend{std::move(backend)}
    , mScheduler{std::move(scheduler)}
    , mService{std::make_unique<MusicBrainzService>(*mBackend, *mScheduler)}
{
}

OnlineLookup::~OnlineLookup() = default;

std::shared_ptr<Request> OnlineLookup::requestArtwork(QString const& artist, QString const& album)
{
    auto request = std::make_shared<Request>();
    auto const key = albumKey(artist, album);

    if (auto const result = mResults.constFind(key); result != mResults.cend()) {
        // Finished later, so that the caller can connect to the request first.
        QTimer::singleShot(0, request.get(), [request = request.get(), image = result.value()] {
            request->finish(image);
        });
        return request;
    }

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
    return request;
}

void OnlineLookup::onLookupFinished(QString const& key)
{
    auto const pending = mPendingLookups.take(key);
    if (pending == nullptr) {
        return;
    }

    auto const image = pending->lookup->image();
    mResults.insert(key, image);
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
