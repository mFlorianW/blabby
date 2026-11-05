// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "OnlineLookupShould.hpp"
#include <QBuffer>
#include <QDir>
#include <QSignalSpy>
#include <QTest>

using namespace std::chrono_literals;

namespace Artwork
{

namespace
{

QByteArray releaseGroupSearch(QString const& id, int score)
{
    return QStringLiteral(R"({"created":"2026-11-05T10:00:00.000Z","count":1,"offset":0,)"
                          R"("release-groups":[{"id":"%1","score":%2,"title":"Low Tide Sessions"}]})")
        .arg(id, QString::number(score))
        .toUtf8();
}

QByteArray cover(QColor const& color, QSize const& size = QSize{500, 500})
{
    auto image = QImage{size, QImage::Format_RGB32};
    image.fill(color);
    auto data = QByteArray{};
    auto buffer = QBuffer{&data};
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return data;
}

constexpr auto lowTideSearch =
    QLatin1StringView{R"(musicbrainz.org/ws/2/release-group/?query=releasegroup:"Low Tide Sessions")"};
constexpr auto harbourSearch =
    QLatin1StringView{R"(musicbrainz.org/ws/2/release-group/?query=releasegroup:"Harbour Nights")"};
constexpr auto saltSearch =
    QLatin1StringView{R"(musicbrainz.org/ws/2/release-group/?query=releasegroup:"Salt & Spray")"};
constexpr auto lowTideCover = QLatin1StringView{"coverartarchive.org/release-group/rg-low-tide/front-500"};
constexpr auto harbourCover = QLatin1StringView{"coverartarchive.org/release-group/rg-harbour/front-500"};

/**
 * Gives true when the color is red, apart from the small deviations of a lossy image format.
 */
bool isRed(QColor const& color)
{
    return color.red() > 240 and color.green() < 16 and color.blue() < 16;
}

} // namespace

OnlineLookupShould::~OnlineLookupShould() = default;

void OnlineLookupShould::init()
{
    QVERIFY(mCacheDirectory.isValid());
    auto backend = std::make_unique<HttpBackendDouble>();
    auto scheduler = std::make_unique<SchedulerDouble>();
    mBackend = backend.get();
    mScheduler = scheduler.get();
    mLookup = std::make_unique<OnlineLookup>(std::move(backend), std::move(scheduler), mCacheDirectory.path());
}

std::unique_ptr<OnlineLookup> OnlineLookupShould::restart(HttpBackendDouble*& backend) const
{
    auto newBackend = std::make_unique<HttpBackendDouble>();
    backend = newBackend.get();
    return std::make_unique<OnlineLookup>(std::move(newBackend),
                                          std::make_unique<SchedulerDouble>(),
                                          mCacheDirectory.path());
}

void OnlineLookupShould::cleanup()
{
    mLookup.reset();
    mBackend = nullptr;
    mScheduler = nullptr;
    // Every test starts with an empty cache.
    QVERIFY(QDir{mCacheDirectory.path()}.removeRecursively());
    QVERIFY(QDir{}.mkpath(mCacheDirectory.path()));
}

void OnlineLookupShould::deliver_the_front_cover_of_the_best_matching_release_group()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().isNull(), false);
    QCOMPARE(request->image().pixelColor(0, 0), QColor{Qt::red});
    auto const urls = mBackend->requestedUrls();
    QCOMPARE(urls.size(), 2);
    QVERIFY(urls.at(0).startsWith(QStringLiteral("https://musicbrainz.org/ws/2/release-group/?")));
    QVERIFY(urls.at(0).contains(QStringLiteral(R"(releasegroup:"Low Tide Sessions" AND artist:"The Quiet Ferries")")));
    QCOMPARE(urls.at(1), QStringLiteral("https://coverartarchive.org/release-group/rg-low-tide/front-500"));
}

void OnlineLookupShould::deliver_no_artwork_without_a_confident_match()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 89));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().isNull(), true);
    QCOMPARE(mBackend->requests().size(), 1);
}

void OnlineLookupShould::demand_a_higher_score_without_an_artist()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 94));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));

    auto const request = mLookup->requestArtwork(QString{}, QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().isNull(), true);
    QCOMPARE(mBackend->requests().size(), 1);
    QVERIFY(not mBackend->requestedUrls().at(0).contains(QStringLiteral("artist:")));
}

void OnlineLookupShould::deliver_no_artwork_for_a_release_group_without_a_front_cover()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 404);

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().isNull(), true);
    QCOMPARE(mBackend->requests().size(), 2);
}

void OnlineLookupShould::deliver_no_artwork_without_internet_access()
{
    mBackend->setResponse(lowTideSearch, 0);

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().isNull(), true);
    QCOMPARE(mBackend->requests().size(), 1);
}

void OnlineLookupShould::send_at_most_one_musicbrainz_request_per_second()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));
    mBackend->setResponse(harbourSearch, 200, releaseGroupSearch(QStringLiteral("rg-harbour"), 100));
    mBackend->setResponse(harbourCover, 200, cover(Qt::blue));

    auto const lowTide =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto const harbour = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Harbour Nights"));
    auto lowTideSpy = QSignalSpy{lowTide.get(), &Request::finished};
    auto harbourSpy = QSignalSpy{harbour.get(), &Request::finished};

    // One lookup finishes, the MusicBrainz search of the other one waits for a second.
    QTRY_COMPARE(lowTideSpy.size() + harbourSpy.size(), 1);
    QTest::qWait(10);
    QCOMPARE(mBackend->requests().size(), 2);
    mScheduler->advance(999ms);
    QTest::qWait(10);
    QCOMPARE(mBackend->requests().size(), 2);

    mScheduler->advance(1ms);

    QTRY_COMPARE(lowTideSpy.size() + harbourSpy.size(), 2);
    QCOMPARE(mBackend->requests().size(), 4);
    QCOMPARE(lowTide->image().pixelColor(0, 0), QColor{Qt::red});
    QCOMPARE(harbour->image().pixelColor(0, 0), QColor{Qt::blue});
}

void OnlineLookupShould::search_the_newest_request_first()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 10));
    mBackend->setResponse(harbourSearch, 200, releaseGroupSearch(QStringLiteral("rg-harbour"), 10));
    mBackend->setResponse(saltSearch, 200, releaseGroupSearch(QStringLiteral("rg-salt"), 10));
    auto const lowTide =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto const harbour = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Harbour Nights"));
    auto const salt = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Salt & Spray"));
    auto lowTideSpy = QSignalSpy{lowTide.get(), &Request::finished};
    QVERIFY(lowTideSpy.wait());

    mScheduler->advance(1s);
    QTRY_COMPARE(mBackend->requests().size(), 2);
    mScheduler->advance(1s);
    QTRY_COMPARE(mBackend->requests().size(), 3);

    auto const urls = mBackend->requestedUrls();
    QVERIFY(urls.at(0).contains(lowTideSearch));
    QVERIFY(urls.at(1).contains(saltSearch));
    QVERIFY(urls.at(2).contains(harbourSearch));
}

void OnlineLookupShould::pause_the_searches_when_musicbrainz_is_overloaded()
{
    mBackend->setResponse(lowTideSearch, 503);
    mBackend->setResponse(harbourSearch, 200, releaseGroupSearch(QStringLiteral("rg-harbour"), 10));
    auto const lowTide =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto const harbour = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Harbour Nights"));
    auto lowTideSpy = QSignalSpy{lowTide.get(), &Request::finished};
    QVERIFY(lowTideSpy.wait());
    QCOMPARE(lowTide->image().isNull(), true);

    mScheduler->advance(4999ms);
    QTest::qWait(10);
    QCOMPARE(mBackend->requests().size(), 1);

    mScheduler->advance(1ms);

    QTRY_COMPARE(mBackend->requests().size(), 2);
    QVERIFY(mBackend->requestedUrls().at(1).contains(harbourSearch));
}

void OnlineLookupShould::identify_itself_with_a_user_agent()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(mBackend->requests().size(), 2);
    for (auto const& sent : mBackend->requests()) {
        auto const userAgent = QString::fromUtf8(sent.rawHeader("User-Agent"));
        QVERIFY2(userAgent.startsWith(QStringLiteral("Blabby/")), qPrintable(userAgent));
        QVERIFY2(userAgent.contains(QStringLiteral("https://github.com/mFlorianW/blabby")), qPrintable(userAgent));
    }
}

void OnlineLookupShould::look_up_an_album_only_once()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));
    auto const first =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto firstSpy = QSignalSpy{first.get(), &Request::finished};
    QVERIFY(firstSpy.wait());

    auto const second =
        mLookup->requestArtwork(QStringLiteral("the quiet ferries "), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{second.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QVERIFY(isRed(second->image().pixelColor(0, 0)));
    QCOMPARE(mBackend->requests().size(), 2);
}

void OnlineLookupShould::deliver_the_artwork_to_all_requests_of_an_album()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));

    auto const first =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto const second =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto firstSpy = QSignalSpy{first.get(), &Request::finished};
    auto secondSpy = QSignalSpy{second.get(), &Request::finished};

    QTRY_COMPARE(firstSpy.size() + secondSpy.size(), 2);
    QCOMPARE(first->image().pixelColor(0, 0), QColor{Qt::red});
    QCOMPARE(second->image().pixelColor(0, 0), QColor{Qt::red});
    QCOMPARE(mBackend->requests().size(), 2);
}

void OnlineLookupShould::not_look_up_a_cancelled_request()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(harbourSearch, 200, releaseGroupSearch(QStringLiteral("rg-harbour"), 100));
    auto const lowTide =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto const harbour = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Harbour Nights"));
    auto lowTideSpy = QSignalSpy{lowTide.get(), &Request::finished};
    auto harbourSpy = QSignalSpy{harbour.get(), &Request::finished};
    QTRY_COMPARE(lowTideSpy.size() + harbourSpy.size(), 1);
    auto const& waiting = lowTideSpy.isEmpty() ? lowTide : harbour;

    waiting->cancel();
    mScheduler->advance(1s);
    QTest::qWait(10);

    QCOMPARE(waiting->isCancelled(), true);
    QCOMPARE(lowTideSpy.size() + harbourSpy.size(), 1);
    QCOMPARE(mBackend->requests().size(), 2);
}

void OnlineLookupShould::not_look_up_a_request_cancelled_while_reading_the_cache()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    auto const request = mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    request->cancel();
    QTest::qWait(50);

    QCOMPARE(finishedSpy.size(), 0);
    QCOMPARE(mBackend->requests().size(), 0);
}

void OnlineLookupShould::serve_found_artwork_from_disk_after_a_restart()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));
    auto const first =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto firstSpy = QSignalSpy{first.get(), &Request::finished};
    QVERIFY(firstSpy.wait());
    mLookup.reset();

    auto* backend = static_cast<HttpBackendDouble*>(nullptr);
    auto const restarted = restart(backend);
    auto const request =
        restarted->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QVERIFY(isRed(request->image().pixelColor(0, 0)));
    QCOMPARE(backend->requests().size(), 0);
}

void OnlineLookupShould::match_the_cached_artwork_regardless_of_case_and_surrounding_whitespace()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red));
    auto const first =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto firstSpy = QSignalSpy{first.get(), &Request::finished};
    QVERIFY(firstSpy.wait());
    mLookup.reset();

    auto* backend = static_cast<HttpBackendDouble*>(nullptr);
    auto const restarted = restart(backend);
    auto const request =
        restarted->requestArtwork(QStringLiteral(" the quiet FERRIES"), QStringLiteral("LOW TIDE SESSIONS  "));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QVERIFY(isRed(request->image().pixelColor(0, 0)));
    QCOMPARE(backend->requests().size(), 0);
}

void OnlineLookupShould::look_up_an_album_without_artwork_again_only_after_a_restart_data()
{
    QTest::addColumn<int>("searchStatus");
    QTest::addColumn<int>("score");
    QTest::addColumn<int>("coverStatus");

    QTest::newRow("no confident match") << 200 << 89 << 200;
    QTest::newRow("no front cover") << 200 << 100 << 404;
    QTest::newRow("network error") << 0 << 100 << 200;
    QTest::newRow("MusicBrainz overloaded") << 503 << 100 << 200;
    QTest::newRow("Cover Art Archive failing") << 200 << 100 << 502;
}

void OnlineLookupShould::look_up_an_album_without_artwork_again_only_after_a_restart()
{
    QFETCH(int, searchStatus);
    QFETCH(int, score);
    QFETCH(int, coverStatus);
    auto const script = [&](HttpBackendDouble& backend) {
        backend.setResponse(lowTideSearch, searchStatus, releaseGroupSearch(QStringLiteral("rg-low-tide"), score));
        backend.setResponse(lowTideCover, coverStatus, cover(Qt::red));
    };
    script(*mBackend);
    auto const first =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto firstSpy = QSignalSpy{first.get(), &Request::finished};
    QVERIFY(firstSpy.wait());
    QCOMPARE(first->image().isNull(), true);
    auto const sentRequests = mBackend->requests().size();

    auto const second =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto secondSpy = QSignalSpy{second.get(), &Request::finished};
    QVERIFY(secondSpy.wait());
    QCOMPARE(second->image().isNull(), true);
    QCOMPARE(mBackend->requests().size(), sentRequests);
    mLookup.reset();

    auto* backend = static_cast<HttpBackendDouble*>(nullptr);
    auto const restarted = restart(backend);
    script(*backend);
    auto const afterRestart =
        restarted->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto afterRestartSpy = QSignalSpy{afterRestart.get(), &Request::finished};

    QVERIFY(afterRestartSpy.wait());
    QCOMPARE(backend->requests().size(), sentRequests);
}

void OnlineLookupShould::keep_artwork_of_at_most_500_px()
{
    mBackend->setResponse(lowTideSearch, 200, releaseGroupSearch(QStringLiteral("rg-low-tide"), 100));
    mBackend->setResponse(lowTideCover, 200, cover(Qt::red, QSize{1200, 900}));

    auto const request =
        mLookup->requestArtwork(QStringLiteral("The Quiet Ferries"), QStringLiteral("Low Tide Sessions"));
    auto finishedSpy = QSignalSpy{request.get(), &Request::finished};

    QVERIFY(finishedSpy.wait());
    QCOMPARE(request->image().size(), QSize(500, 375));
    auto const written = QDir{mCacheDirectory.path()}.entryInfoList(QDir::Files);
    QCOMPARE(written.size(), 1);
    QCOMPARE(QImage{written.at(0).filePath()}.size(), QSize(500, 375));
}

} // namespace Artwork

QTEST_GUILESS_MAIN(Artwork::OnlineLookupShould)
