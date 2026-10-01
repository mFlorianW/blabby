// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaItemModel.hpp"
#include "LoggingCategories.hpp"

namespace Shell
{

MediaItemModel::~MediaItemModel() = default;

int MediaItemModel::rowCount(QModelIndex const& index) const noexcept
{
    Q_UNUSED(index);

    return mRowCount;
}

QHash<int, QByteArray> MediaItemModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(DisplayRole::MediaItemTitle), QByteArray{"mediaItemTitle"}),
        std::make_pair(static_cast<int>(DisplayRole::MediaItemArtworkUrl), QByteArray{"mediaItemArtworkUrl"}),
        std::make_pair(static_cast<int>(DisplayRole::MediaItemType), QByteArray{"mediaItemType"}),
        std::make_pair(static_cast<int>(DisplayRole::MediaItemSecondaryText), QByteArray{"mediaItemSecondaryText"}),
    };
    return roles;
}

QVariant MediaItemModel::data(QModelIndex const& index, int role) const noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to request data. Error: MediaSource is not set.";
        return {};
    }

    auto const& items = mMediaSrc->mediaItems();
    if (not index.isValid() or (index.row() >= items.size())) {
        qCritical(shell) << "Failed to request data. Error: Invalided index for requesting data. Index:" << index
                         << "Source size:" << items.size();
        return {};
    }

    auto const& item = items.at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::MediaItemTitle) {
        return item.mainText();
    } else if (dispRole == DisplayRole::MediaItemArtworkUrl) {
        return item.artworkUrl();
    } else if (dispRole == DisplayRole::MediaItemType) {
        return static_cast<int>(item.type());
    } else if (dispRole == DisplayRole::MediaItemSecondaryText) {
        return item.secondaryText();
    }

    return {};
}

bool MediaItemModel::canFetchMore(QModelIndex const& parent) const noexcept
{
    return not parent.isValid() and mMediaSrc != nullptr and not isBusy() and not mLoadingMore and
           not mLoadMoreFailed and mMediaSrc->canLoadMore();
}

void MediaItemModel::fetchMore(QModelIndex const& parent) noexcept
{
    if (not canFetchMore(parent)) {
        return;
    }

    mLoadingMore = true;
    mMediaSrc->loadMore();
}

void MediaItemModel::setMediaSource(std::shared_ptr<Multimedia::Source> const& mediaSrc)
{
    if (mMediaSrc != mediaSrc) {
        if (mMediaSrc != nullptr) {
            disconnect(mMediaSrc.get(), nullptr, this, nullptr);
        }

        auto const wasBusy = isBusy();
        auto const wasAtRoot = isAtRoot();
        beginResetModel();
        mMediaSrc = mediaSrc;
        mRowCount = mMediaSrc != nullptr ? static_cast<int>(mMediaSrc->mediaItems().size()) : 0;
        mContainerTitles.clear();
        mPendingNavigation = PendingNavigation::None;
        mPendingContainerTitle.clear();
        mLoadingMore = false;
        endResetModel();

        if (mMediaSrc != nullptr) {
            connect(mMediaSrc.get(),
                    &Multimedia::Source::navigationFinished,
                    this,
                    &MediaItemModel::onNavigationFinished);
            connect(mMediaSrc.get(), &Multimedia::Source::navigationFailed, this, &MediaItemModel::onNavigationFailed);
            connect(mMediaSrc.get(), &Multimedia::Source::moreItemsLoaded, this, &MediaItemModel::onMoreItemsLoaded);
            connect(
                mMediaSrc.get(), &Multimedia::Source::loadingMoreFailed, this, &MediaItemModel::onLoadingMoreFailed);
        }
        setLoadMoreFailed(false);
        Q_EMIT mediaSourceChanged();
        if (wasBusy) {
            Q_EMIT busyChanged();
        }
        if (not wasAtRoot) {
            Q_EMIT containerChanged();
        }
    }
}

void MediaItemModel::activateMediaItem(qsizetype idx) noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to activate MediaItem. Error: MediaSource is not set.";
        return;
    }

    if (isBusy()) {
        return;
    }

    auto const& items = mMediaSrc->mediaItems();
    if (idx >= items.size() or idx < 0) {
        qCritical(shell) << "Failed to activate MediaItem. Error: Invalid MediaItem index" << idx;
        return;
    }

    auto const& item = items.at(idx);
    if (item.type() == Multimedia::ItemType::Container) {
        // Copy the path, the Source may replace its Items while navigating.
        auto const path = item.path();
        startNavigation(PendingNavigation::Open, item.mainText());
        mMediaSrc->navigateTo(path);
        return;
    }

    Q_EMIT playRequested(item);
}

void MediaItemModel::navigateBack() noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to navigate back. Error: MediaSource is not set.";
        return;
    }

    if (isBusy() or isAtRoot()) {
        return;
    }

    startNavigation(PendingNavigation::Back, parentContainerTitle());
    mMediaSrc->navigateBack();
}

void MediaItemModel::retryLoadMore() noexcept
{
    if (not mLoadMoreFailed) {
        return;
    }

    setLoadMoreFailed(false);
    fetchMore({});
}

void MediaItemModel::startNavigation(PendingNavigation navigation, QString const& containerTitle) noexcept
{
    mPendingNavigation = navigation;
    mPendingContainerTitle = containerTitle;
    Q_EMIT busyChanged();
}

void MediaItemModel::onNavigationFinished() noexcept
{
    beginResetModel();
    mRowCount = static_cast<int>(mMediaSrc->mediaItems().size());
    mLoadingMore = false;
    endResetModel();
    setLoadMoreFailed(false);

    auto const navigation = std::exchange(mPendingNavigation, PendingNavigation::None);
    if (navigation == PendingNavigation::Open) {
        mContainerTitles.append(std::exchange(mPendingContainerTitle, {}));
        Q_EMIT containerChanged();
    } else if (navigation == PendingNavigation::Back) {
        mPendingContainerTitle.clear();
        mContainerTitles.removeLast();
        Q_EMIT containerChanged();
    }

    if (navigation != PendingNavigation::None) {
        Q_EMIT busyChanged();
    }
}

void MediaItemModel::onNavigationFailed() noexcept
{
    // The Source drops the page it was loading when it navigates, the current Container misses that page then.
    if (std::exchange(mLoadingMore, false)) {
        setLoadMoreFailed(true);
    }

    auto const navigation = std::exchange(mPendingNavigation, PendingNavigation::None);
    if (navigation == PendingNavigation::None) {
        return;
    }

    Q_EMIT busyChanged();
    Q_EMIT containerOpenFailed(std::exchange(mPendingContainerTitle, {}));
}

void MediaItemModel::onMoreItemsLoaded() noexcept
{
    mLoadingMore = false;
    auto const itemCount = static_cast<int>(mMediaSrc->mediaItems().size());
    if (itemCount > mRowCount) {
        beginInsertRows({}, mRowCount, itemCount - 1);
        mRowCount = itemCount;
        endInsertRows();
    }
}

void MediaItemModel::onLoadingMoreFailed() noexcept
{
    mLoadingMore = false;
    setLoadMoreFailed(true);
}

void MediaItemModel::setLoadMoreFailed(bool failed) noexcept
{
    if (mLoadMoreFailed != failed) {
        mLoadMoreFailed = failed;
        Q_EMIT loadMoreFailedChanged();
    }
}

QString MediaItemModel::mediaSourceName() const noexcept
{
    if (mMediaSrc != nullptr) {
        return mMediaSrc->sourceName();
    }
    return {};
}

QString MediaItemModel::mediaSourceIconUrl() const noexcept
{
    if (mMediaSrc != nullptr) {
        return mMediaSrc->iconUrl();
    }
    return {};
}

bool MediaItemModel::hasMediaSource() const noexcept
{
    return mMediaSrc != nullptr;
}

bool MediaItemModel::isBusy() const noexcept
{
    return mPendingNavigation != PendingNavigation::None;
}

QString MediaItemModel::containerTitle() const noexcept
{
    return mContainerTitles.isEmpty() ? QString{} : mContainerTitles.last();
}

QString MediaItemModel::parentContainerTitle() const noexcept
{
    // The root Container has no title of its own, it's named after the Source.
    return mContainerTitles.size() > 1 ? mContainerTitles.at(mContainerTitles.size() - 2) : mMediaSrc->sourceName();
}

bool MediaItemModel::isAtRoot() const noexcept
{
    return mContainerTitles.isEmpty();
}

bool MediaItemModel::hasLoadMoreFailed() const noexcept
{
    return mLoadMoreFailed;
}

} // namespace Shell
