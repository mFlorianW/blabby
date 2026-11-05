// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "HttpBackendDouble.hpp"
#include "Lookup.hpp"
#include "SchedulerDouble.hpp"
#include <QObject>
#include <QTemporaryDir>
#include <memory>

namespace Artwork
{

class OnlineLookupShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~OnlineLookupShould() override;
    Q_DISABLE_COPY_MOVE(OnlineLookupShould)

private:
    /**
     * Creates another instance on the same cache directory, as after a restart of Blabby.
     */
    std::unique_ptr<OnlineLookup> restart(HttpBackendDouble*& backend) const;

    /**
     * Replaces the instance by one on the same cache directory whose cache keeps at most the given number of bytes.
     */
    void startWithCacheCapacity(qint64 cacheCapacity);

    /**
     * Caches the Artwork of "Low Tide Sessions", then restarts with a cache that has room for two albums.
     */
    void cacheOneAlbumAndRestartWithRoomForTwo();

    /**
     * Requests the red Artwork of an album, after the rate limit of the previous search, and waits for it.
     * @return The number of HTTP requests that were sent for it, -1 when no red Artwork was delivered.
     */
    qsizetype requestsSentToGetArtwork(QString const& album);

    QTemporaryDir mCacheDirectory;
    HttpBackendDouble* mBackend = nullptr;
    SchedulerDouble* mScheduler = nullptr;
    std::unique_ptr<OnlineLookup> mLookup;

private Q_SLOTS:
    void init();
    void cleanup();
    void deliver_the_front_cover_of_the_best_matching_release_group();
    void deliver_no_artwork_without_a_confident_match();
    void demand_a_higher_score_without_an_artist();
    void deliver_no_artwork_for_a_release_group_without_a_front_cover();
    void deliver_no_artwork_without_internet_access();
    void send_at_most_one_musicbrainz_request_per_second();
    void search_the_newest_request_first();
    void pause_the_searches_when_musicbrainz_is_overloaded();
    void identify_itself_with_a_user_agent();
    void look_up_an_album_only_once();
    void deliver_the_artwork_to_all_requests_of_an_album();
    void not_look_up_a_cancelled_request();
    void not_look_up_a_request_cancelled_while_reading_the_cache();
    void serve_found_artwork_from_disk_after_a_restart();
    void match_the_cached_artwork_regardless_of_case_and_surrounding_whitespace();
    void look_up_an_album_without_artwork_again_only_after_a_restart_data();
    void look_up_an_album_without_artwork_again_only_after_a_restart();
    void keep_artwork_of_at_most_500_px();
    void remove_the_least_recently_used_artwork_when_the_cache_is_full();
    void count_reading_the_cached_artwork_as_a_use();
};

} // namespace Artwork
