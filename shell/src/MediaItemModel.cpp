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

    if (mMediaSrc != nullptr) {
        return static_cast<int>(mMediaSrc->mediaItems().size());
    }

    return int{0};
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
        mContainerTitles.clear();
        mPendingNavigation = PendingNavigation::None;
        mPendingContainerTitle.clear();
        endResetModel();

        if (mMediaSrc != nullptr) {
            connect(mMediaSrc.get(),
                    &Multimedia::Source::navigationFinished,
                    this,
                    &MediaItemModel::onNavigationFinished);
            connect(mMediaSrc.get(), &Multimedia::Source::navigationFailed, this, &MediaItemModel::onNavigationFailed);
        }
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

void MediaItemModel::startNavigation(PendingNavigation navigation, QString const& containerTitle) noexcept
{
    mPendingNavigation = navigation;
    mPendingContainerTitle = containerTitle;
    Q_EMIT busyChanged();
}

void MediaItemModel::onNavigationFinished() noexcept
{
    beginResetModel();
    endResetModel();

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
    auto const navigation = std::exchange(mPendingNavigation, PendingNavigation::None);
    if (navigation == PendingNavigation::None) {
        return;
    }

    Q_EMIT busyChanged();
    Q_EMIT containerOpenFailed(std::exchange(mPendingContainerTitle, {}));
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

} // namespace Shell
