// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItemModel.hpp"
#include "LoggingCategories.hpp"

namespace Shell
{

MediaItemModel::~MediaItemModel() = default;

int MediaItemModel::rowCount(QModelIndex const &index) const noexcept
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
        std::make_pair(static_cast<int>(DisplayRole::MediaItemIconUrl), QByteArray{"mediaItemIconUrl"}),
    };
    return roles;
}

QVariant MediaItemModel::data(QModelIndex const &index, int role) const noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to request data. Error: MediaSource is not set.";
        return {};
    }

    auto const &items = mMediaSrc->mediaItems();
    if (not index.isValid() or (index.row() >= items.size())) {
        qCritical(shell) << "Failed to request data. Error: Invalided index for requesting data. Index:" << index
                         << "Source size:" << items.size();
        return {};
    }

    auto const &item = items.at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::MediaItemTitle) {
        return item.mainText();
    } else if (dispRole == DisplayRole::MediaItemIconUrl) {
        const auto iconUrl = item.iconUrl();
        if (iconUrl.isEmpty()) {
            return item.type() == Multimedia::ItemType::Container
                       ? QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/24x24/folder.svg")
                       : QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg");
        }
    }

    return {};
}

void MediaItemModel::setMediaSource(std::shared_ptr<Multimedia::MediaSource> const &mediaSrc)
{
    if (mMediaSrc != mediaSrc) {
        if (mMediaSrc != nullptr) {
            disconnect(mediaSrc.get());
        }

        beginResetModel();
        mMediaSrc = mediaSrc;
        endResetModel();

        if (mMediaSrc != nullptr) {
            connect(mMediaSrc.get(), &Multimedia::MediaSource::navigationFinished, this, [&] {
                beginResetModel();
                endResetModel();
            });
        }
        Q_EMIT mediaSourceChanged();
    }
}

void MediaItemModel::activateMediaItem(qsizetype idx) noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to activate MediaItem. Error: MediaSource is not set.";
        return;
    }

    const auto &items = mMediaSrc->mediaItems();
    if (idx >= items.size() or idx < 0) {
        qCritical(shell) << "Failed to activate MediaItem. Error: Invalid MediaItem index" << idx;
        return;
    }

    const auto &item = items.at(idx);
    if (item.type() == Multimedia::ItemType::Container) {
        mMediaSrc->navigateTo(item.path());
    } else {
        Q_EMIT playRequest(item);
    }
}

void MediaItemModel::navigateBack() const noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to navigate back. Error: MediaSource is not set.";
        return;
    }
    mMediaSrc->navigateBack();
}

void MediaItemModel::navigateForward() const noexcept
{
    if (mMediaSrc == nullptr) {
        qCritical(shell) << "Failed to navigate forward. Error: MediaSource is not set.";
        return;
    }
    mMediaSrc->navigateForward();
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

} // namespace Shell
