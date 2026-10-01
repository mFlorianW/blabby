// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include <QHash>

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

protected:
    void navigate(QString const& path, qsizetype minimumItemCount) noexcept override;

private:
    void finishNavigation(QString const& path) noexcept;

    QHash<QString, Items> mItems;
    QString mLastNavigationPath;
    qsizetype mLastMinimumItemCount{0};
    qsizetype mNavigationCount{0};
    bool mHoldNavigations{false};
    QString mPendingPath;
    Items mMoreItems;
    qsizetype mLoadMoreCount{0};
};

} // namespace Multimedia::TestHelper
