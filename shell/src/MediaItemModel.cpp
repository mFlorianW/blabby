// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaItemModel.hpp"
#include "LoggingCategories.hpp"
#include <algorithm>

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
        mOpenedContainers.clear();
        mPendingNavigation = PendingNavigation::None;
        mPendingContainer = {};
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

void MediaItemModel::activateMediaItem(qsizetype idx, qsizetype scrollPosition) noexcept
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
        startNavigation(PendingNavigation::Open,
                        {.mTitle = item.mainText(), .mParentItemCount = mRowCount, .mParentScrollPosition = scrollPosition});
        mMediaSrc->navigateTo(path);
        return;
    }

    Q_EMIT playRequested(item);
}

void MediaItemModel::playMediaItemNext(qsizetype idx) noexcept
{
    if (auto const item = itemForQueue(idx); item.has_value()) {
        Q_EMIT playNextRequested(mMediaSrc, *item);
    }
}

void MediaItemModel::addMediaItemToQueue(qsizetype idx) noexcept
{
    if (auto const item = itemForQueue(idx); item.has_value()) {
        Q_EMIT addToQueueRequested(mMediaSrc, *item);
    }
}

std::optional<Multimedia::Item> MediaItemModel::itemForQueue(qsizetype idx) const noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to add a MediaItem to the Queue. Error: MediaSource is not set.";
        return std::nullopt;
    }

    if (isBusy()) {
        return std::nullopt;
    }

    auto const& items = mMediaSrc->mediaItems();
    if (idx >= items.size() or idx < 0) {
        qCritical(shell) << "Failed to add a MediaItem to the Queue. Error: Invalid MediaItem index" << idx;
        return std::nullopt;
    }
    return items.at(idx);
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

    startNavigation(PendingNavigation::Back, {.mTitle = parentContainerTitle()});
    // Items are re-fetched instead of cached, at least as many as the parent Container had loaded.
    mMediaSrc->navigateBack(mOpenedContainers.last().mParentItemCount);
}

void MediaItemModel::retryLoadMore() noexcept
{
    if (not mLoadMoreFailed) {
        return;
    }

    setLoadMoreFailed(false);
    fetchMore({});
}

void MediaItemModel::startNavigation(PendingNavigation navigation, OpenedContainer container) noexcept
{
    mPendingNavigation = navigation;
    mPendingContainer = std::move(container);
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
    auto const container = std::exchange(mPendingContainer, {});
    if (navigation == PendingNavigation::Open) {
        mOpenedContainers.append(container);
        Q_EMIT containerChanged();
        Q_EMIT busyChanged();
    } else if (navigation == PendingNavigation::Back) {
        auto const scrollPosition = mOpenedContainers.takeLast().mParentScrollPosition;
        Q_EMIT containerChanged();
        Q_EMIT busyChanged();
        // The Container may have shrunk meanwhile, the closest valid position is its last Item.
        Q_EMIT scrollPositionRestoreRequested(std::max(qsizetype{0}, std::min(scrollPosition, qsizetype{mRowCount} - 1)));
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
    Q_EMIT containerOpenFailed(std::exchange(mPendingContainer, {}).mTitle);
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
    return mOpenedContainers.isEmpty() ? QString{} : mOpenedContainers.last().mTitle;
}

QString MediaItemModel::parentContainerTitle() const noexcept
{
    // The root Container has no title of its own, it's named after the Source.
    return mOpenedContainers.size() > 1 ? mOpenedContainers.at(mOpenedContainers.size() - 2).mTitle : mMediaSrc->sourceName();
}

bool MediaItemModel::isAtRoot() const noexcept
{
    return mOpenedContainers.isEmpty();
}

bool MediaItemModel::hasLoadMoreFailed() const noexcept
{
    return mLoadMoreFailed;
}

} // namespace Shell
