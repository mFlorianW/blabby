// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SourceShould.hpp"
#include "TestSource.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace Multimedia::TestHelper;

namespace Multimedia
{

namespace
{
/**
 * A Source that isn't paged and keeps the default implementations.
 */
class PlainSource final : public Source
{
public:
    PlainSource()
        : Source{QStringLiteral("Plain")}
    {
    }
};
} // namespace

MediaSourceShould::MediaSourceShould() = default;
MediaSourceShould::~MediaSourceShould() = default;

void MediaSourceShould::give_the_name_of_media_source()
{
    auto const mediaSource = TestSource{QStringLiteral("MusicBox"), QString{}};
    auto const expName = QStringLiteral("MusicBox");

    QVERIFY2(mediaSource.sourceName() == expName,
             QString("The MediaSource name \"%1\" is not the expected one %2")
                 .arg(mediaSource.sourceName(), expName)
                 .toLocal8Bit());
}

void MediaSourceShould::give_a_icon_url_when_set()
{
    auto const mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    auto const expUrl = QStringLiteral("http://localhost/musicbox.png");

    QVERIFY2(mediaSource.iconUrl() == expUrl,
             QString("The MediaSource iconUrl \"%1\" is not the expected one %2")
                 .arg(mediaSource.iconUrl(), expUrl)
                 .toLocal8Bit());
}

void MediaSourceShould::navigate_to_previous_layer()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
    auto const path1 = QStringLiteral("1");
    mediaSource.navigateTo(path1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);
    auto const path2 = QStringLiteral("2");
    mediaSource.navigateTo(path2);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("2"));
    auto navSignalSpy = QSignalSpy{&mediaSource, &Source::navigationFinished};
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
}

void MediaSourceShould::navigate_forward_to_previous_layer()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    auto const path1 = QStringLiteral("1");
    auto const path2 = QStringLiteral("2");

    mediaSource.navigateTo(path1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);
    mediaSource.navigateTo(path2);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    auto navSignalSpy = QSignalSpy{&mediaSource, &Source::navigationFinished};
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    navSignalSpy.clear();
    mediaSource.navigateForward();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateForward();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path2);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));

    mediaSource.navigateTo(path1);
    mediaSource.navigateTo(path2);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), path1);

    navSignalSpy.clear();
    mediaSource.navigateBack();
    QCOMPARE(navSignalSpy.size(), 1);
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
}

void MediaSourceShould::keep_the_navigation_history_when_navigating_back_fails()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QStringLiteral("http://localhost/musicbox.png")};
    mediaSource.navigateTo(QStringLiteral("1"));
    mediaSource.navigateTo(QStringLiteral("2"));
    mediaSource.setHoldNavigations(true);
    mediaSource.navigateBack();
    mediaSource.failPendingNavigation();
    mediaSource.setHoldNavigations(false);

    mediaSource.navigateBack();
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("1"));

    // A navigation after the failed one is recorded as usual.
    mediaSource.navigateTo(QStringLiteral("2"));
    mediaSource.navigateBack();
    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("1"));
}

void MediaSourceShould::have_no_more_items_to_load_unless_paged()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QString{}};

    QCOMPARE(mediaSource.totalItemCount(), mediaSource.mediaItems().size());
    QCOMPARE(mediaSource.canLoadMore(), false);
}

void MediaSourceShould::navigate_with_a_minimum_item_count()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QString{}};
    QCOMPARE(mediaSource.lastMinimumItemCount(), 0);

    mediaSource.navigateTo(QStringLiteral("1"), 250);

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("1"));
    QCOMPARE(mediaSource.lastMinimumItemCount(), 250);
}

void MediaSourceShould::navigate_back_with_a_minimum_item_count()
{
    auto mediaSource = TestSource{QStringLiteral("MusicBox"), QString{}};
    mediaSource.navigateTo(QStringLiteral("1"));

    mediaSource.navigateBack(250);

    QCOMPARE(mediaSource.lastNavigatedPath(), QStringLiteral("0"));
    QCOMPARE(mediaSource.lastMinimumItemCount(), 250);

    // Navigating forward reloads the first page only.
    mediaSource.navigateForward();
    QCOMPARE(mediaSource.lastMinimumItemCount(), 0);
}

void MediaSourceShould::give_a_finished_page_without_items_unless_paged()
{
    auto source = PlainSource{};

    auto const page = source.browsePage(QStringLiteral("1"), 0);

    QCOMPARE(page->isFinished(), true);
    QCOMPARE(page->hasFailed(), false);
    QCOMPARE(page->items(), Items{});
}

void MediaSourceShould::finish_a_page_only_once()
{
    auto page = PendingPage{};
    auto const finishedSpy = QSignalSpy{&page, &PendingPage::finished};

    page.finish({Item{ItemType::Playable, QStringLiteral("Song")}}, 3);
    page.fail();
    page.finish({}, 0);

    QCOMPARE(finishedSpy.size(), 1);
    QCOMPARE(page.hasFailed(), false);
    auto const expectedItems = Items{Item{ItemType::Playable, QStringLiteral("Song")}};
    QCOMPARE(page.items(), expectedItems);
    QCOMPARE(page.totalItemCount(), 3);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::MediaSourceShould);
