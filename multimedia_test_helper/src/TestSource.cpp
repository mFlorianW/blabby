// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestSource.hpp"
#include "LoggingCategories.hpp"
#include <QDebug>
#include <algorithm>

namespace Multimedia::TestHelper
{

TestSource::TestSource(QString name, QString iconUrl)
    : Multimedia::Source{std::move(name), std::move(iconUrl)}
{
    mItems.insert(QStringLiteral("0"),
                  {Multimedia::Item{Multimedia::ItemType::Playable,
                                    QStringLiteral("MediaItem1"),
                                    QStringLiteral("Artist1"),
                                    QStringLiteral("http://localhost/art1.jpg")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem2")},
                   Multimedia::Item{Multimedia::ItemType::Container,
                                    QStringLiteral("Container1"),
                                    QString(""),
                                    QString(""),
                                    QStringLiteral("1")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem4")}});
    mItems.insert(QStringLiteral("1"),
                  {Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem4")},
                   Multimedia::Item{Multimedia::ItemType::Container,
                                    QStringLiteral("Container2"),
                                    QString(""),
                                    QString(""),
                                    QStringLiteral("2")}});
    mItems.insert(QStringLiteral("2"),
                  {Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem5")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem6")}});
    navigateTo(QStringLiteral("0"));
}

TestSource::~TestSource() = default;

void TestSource::navigate(QString const& path, qsizetype minimumItemCount) noexcept
{
    mLastNavigationPath = path;
    mLastMinimumItemCount = minimumItemCount;
    ++mNavigationCount;
    if (mHoldNavigations) {
        mPendingPath = path;
        return;
    }
    finishNavigation(path);
}

void TestSource::finishNavigation(QString const& path) noexcept
{
    mMoreItems.clear();
    setTotalItemCount(0);
    if (mItems.contains(path)) {
        mMediaItems = mItems[path];
    } else {
        qCCritical(testMediaSource) << "Path not found. Error: Invalied Path" << path << "passed";
    }
    Q_EMIT navigationFinished(path);
}

QString const& TestSource::lastNavigatedPath() const noexcept
{
    return mLastNavigationPath;
}

qsizetype TestSource::lastMinimumItemCount() const noexcept
{
    return mLastMinimumItemCount;
}

qsizetype TestSource::navigationCount() const noexcept
{
    return mNavigationCount;
}

void TestSource::setHoldNavigations(bool hold) noexcept
{
    mHoldNavigations = hold;
}

void TestSource::finishPendingNavigation() noexcept
{
    finishNavigation(mPendingPath);
}

void TestSource::failPendingNavigation() noexcept
{
    Q_EMIT navigationFailed(mPendingPath);
}

void TestSource::loadMore() noexcept
{
    ++mLoadMoreCount;
}

void TestSource::setMoreItems(Items items) noexcept
{
    mMoreItems = std::move(items);
    setTotalItemCount(mMediaItems.size() + mMoreItems.size());
}

qsizetype TestSource::loadMoreCount() const noexcept
{
    return mLoadMoreCount;
}

void TestSource::finishPendingLoadMore() noexcept
{
    mMediaItems.append(std::exchange(mMoreItems, {}));
    Q_EMIT moreItemsLoaded();
}

void TestSource::failPendingLoadMore() noexcept
{
    Q_EMIT loadingMoreFailed();
}

std::unique_ptr<PendingPage> TestSource::browsePage(QString const& path, qsizetype startIndex) noexcept
{
    ++mBrowsedPageCount;
    auto page = std::make_unique<PendingPage>();
    if (mHoldPages) {
        mHeldPages.append(HeldPage{.mPage = page.get(), .mPath = path, .mStartIndex = startIndex});
    } else {
        finishPage(*page, path, startIndex);
    }
    return page;
}

void TestSource::setItems(QString const& path, Items items) noexcept
{
    mItems.insert(path, std::move(items));
}

void TestSource::setPageSize(qsizetype pageSize) noexcept
{
    mPageSize = pageSize;
}

void TestSource::setHoldPages(bool hold) noexcept
{
    mHoldPages = hold;
}

qsizetype TestSource::browsedPageCount() const noexcept
{
    return mBrowsedPageCount;
}

qsizetype TestSource::pendingPageCount() const noexcept
{
    return std::ranges::count_if(mHeldPages, [](HeldPage const& held) {
        return not held.mPage.isNull();
    });
}

void TestSource::finishPendingPage() noexcept
{
    if (auto held = takePendingPage(); held.has_value()) {
        finishPage(*held->mPage, held->mPath, held->mStartIndex);
    }
}

void TestSource::failPendingPage() noexcept
{
    if (auto held = takePendingPage(); held.has_value()) {
        held->mPage->fail();
    }
}

void TestSource::finishPage(PendingPage& page, QString const& path, qsizetype startIndex) const noexcept
{
    if (not mItems.contains(path)) {
        qCCritical(testMediaSource) << "Path not found. Error: Invalied Path" << path << "passed";
        page.fail();
        return;
    }

    auto const& items = mItems[path];
    auto const count = mPageSize > 0 ? mPageSize : items.size();
    page.finish(items.mid(startIndex, count), items.size());
}

std::optional<TestSource::HeldPage> TestSource::takePendingPage() noexcept
{
    while (not mHeldPages.isEmpty()) {
        auto held = mHeldPages.takeFirst();
        if (not held.mPage.isNull()) {
            return held;
        }
    }
    return std::nullopt;
}

} // namespace Multimedia::TestHelper
