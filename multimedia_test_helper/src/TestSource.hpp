// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include <QHash>
#include <QPointer>

namespace Multimedia::TestHelper
{

class TestSource : public Multimedia::Source
{
    Q_OBJECT
public:
    TestSource(QString name, QString iconUrl);
    ~TestSource() override;
    Q_DISABLE_COPY_MOVE(TestSource)

    /**
     * Holds the request back, it only finishes on @ref finishPendingLoadMore() or fails on @ref failPendingLoadMore().
     */
    void loadMore() noexcept override;

    QString const& lastNavigatedPath() const noexcept;

    /**
     * Gives the minimum number of Items requested by the last navigation.
     */
    qsizetype lastMinimumItemCount() const noexcept;

    /**
     * Gives how often a navigation was requested.
     */
    qsizetype navigationCount() const noexcept;

    /**
     * When true, a navigation only finishes on @ref finishPendingNavigation(), like a Source waiting for a server.
     */
    void setHoldNavigations(bool hold) noexcept;

    /**
     * Finishes the navigation held back by @ref setHoldNavigations(bool).
     */
    void finishPendingNavigation() noexcept;

    /**
     * Fails the navigation held back by @ref setHoldNavigations(bool), the Items stay unchanged.
     */
    void failPendingNavigation() noexcept;

    /**
     * Sets the Items of the current Container that aren't loaded yet, they are loaded as one page by
     * @ref loadMore(). A navigation drops them.
     */
    void setMoreItems(Items items) noexcept;

    /**
     * Gives how often loading more Items was requested.
     */
    qsizetype loadMoreCount() const noexcept;

    /**
     * Appends the Items set by @ref setMoreItems(Items) as the page requested by @ref loadMore().
     */
    void finishPendingLoadMore() noexcept;

    /**
     * Fails the page requested by @ref loadMore(), the Items stay unchanged.
     */
    void failPendingLoadMore() noexcept;

    /**
     * Gives the pages of the Container at the path, @ref setPageSize(qsizetype) Items each. A page of a path without
     * Items fails. The page is finished when it's given, unless pages are held, see @ref setHoldPages(bool).
     */
    std::unique_ptr<PendingPage> browsePage(QString const& path, qsizetype startIndex) noexcept override;

    /**
     * Sets the Items of the Container at the path, replacing the ones it had.
     */
    void setItems(QString const& path, Items items) noexcept;

    /**
     * Sets how many Items a page of @ref browsePage(QString const&, qsizetype) holds at most, 0 for all Items.
     */
    void setPageSize(qsizetype pageSize) noexcept;

    /**
     * When true, a page of @ref browsePage(QString const&, qsizetype) only finishes on @ref finishPendingPage() or
     * fails on @ref failPendingPage(), like a Source waiting for a server.
     */
    void setHoldPages(bool hold) noexcept;

    /**
     * Gives how often a page was requested by @ref browsePage(QString const&, qsizetype).
     */
    qsizetype browsedPageCount() const noexcept;

    /**
     * Gives how many held pages wait to finish, pages that were destroyed meanwhile don't count.
     */
    qsizetype pendingPageCount() const noexcept;

    /**
     * Finishes the oldest held page with its Items.
     */
    void finishPendingPage() noexcept;

    /**
     * Fails the oldest held page.
     */
    void failPendingPage() noexcept;

protected:
    void navigate(QString const& path, qsizetype minimumItemCount) noexcept override;

private:
    /**
     * A page held back by @ref setHoldPages(bool).
     */
    struct HeldPage
    {
        QPointer<PendingPage> mPage;
        QString mPath;
        qsizetype mStartIndex{0};
    };

    void finishNavigation(QString const& path) noexcept;
    void finishPage(PendingPage& page, QString const& path, qsizetype startIndex) const noexcept;
    std::optional<HeldPage> takePendingPage() noexcept;

    QHash<QString, Items> mItems;
    QString mLastNavigationPath;
    qsizetype mLastMinimumItemCount{0};
    qsizetype mNavigationCount{0};
    bool mHoldNavigations{false};
    QString mPendingPath;
    Items mMoreItems;
    qsizetype mLoadMoreCount{0};
    qsizetype mPageSize{0};
    bool mHoldPages{false};
    qsizetype mBrowsedPageCount{0};
    QList<HeldPage> mHeldPages;
};

} // namespace Multimedia::TestHelper
