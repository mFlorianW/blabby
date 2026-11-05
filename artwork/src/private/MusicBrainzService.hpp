// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpBackend.hpp"
#include "Scheduler.hpp"
#include "Service.hpp"
#include <list>
#include <optional>
#include <vector>

namespace Artwork
{

/**
 * The @ref Artwork::MusicBrainzService searches the release group of an album on MusicBrainz and gives its front
 * cover from the Cover Art Archive. A match needs a score of at least 90, or 95 when the artist is unknown. The
 * MusicBrainz searches run one at a time and at most one per second, the newest lookup first. When MusicBrainz is
 * overloaded, the searches pause for a few seconds.
 */
class MusicBrainzService final : public Service
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MusicBrainzService)
public:
    /**
     * Creates an instance of the @ref Artwork::MusicBrainzService.
     * @param backend The backend that sends the HTTP requests, it must outlive the service.
     * @param scheduler The scheduler for the rate limit, it must outlive the service.
     */
    MusicBrainzService(HttpBackend& backend, Scheduler& scheduler);
    ~MusicBrainzService() override;

    std::shared_ptr<Request> lookUp(QString const& artist, QString const& album) override;

private:
    struct PendingSearch
    {
        QString artist;
        QString album;
        std::shared_ptr<Request> request;
    };

    struct CoverFetch
    {
        std::unique_ptr<HttpReply> reply;
        std::shared_ptr<Request> request;
    };

    void searchNext();
    void onSearchFinished();
    void fetchCover(QString const& releaseGroupId, std::shared_ptr<Request> request);
    void onCoverFetched(HttpReply const* reply);
    void onCancelled(Request const* request);

    HttpBackend& mBackend;
    Scheduler& mScheduler;
    std::vector<PendingSearch> mPendingSearches;
    std::unique_ptr<HttpReply> mSearchReply;
    std::shared_ptr<Request> mSearchRequest;
    int mSearchMinimumScore = 0;
    std::optional<Scheduler::TimePoint> mNextSearchAt;
    std::list<CoverFetch> mCoverFetches;
};

} // namespace Artwork
