// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QueueModel.hpp"
#include "LoggingCategories.hpp"

using namespace Multimedia;

namespace Shell
{

QueueModel::QueueModel(MediaRendererModel const& rendererModel)
    : mRendererModel{rendererModel}
{
    connect(&mRendererModel, &MediaRendererModel::activeRendererChanged, this, [this] {
        mQueue.setActiveRenderer(mRendererModel.activeRenderer());
    });
    connect(&mQueue, &Queue::entriesAboutToBeReplaced, this, &QueueModel::beginResetModel);
    connect(&mQueue, &Queue::entriesReplaced, this, [this] {
        // The rows are gone with the reset, the new Current Entry is notified by the Queue afterwards.
        mCurrentRow.reset();
        endResetModel();
    });
    connect(&mQueue, &Queue::currentEntryChanged, this, &QueueModel::onCurrentEntryChanged);
    connect(&mQueue, &Queue::stateChanged, this, &QueueModel::runningChanged);
    mQueue.setActiveRenderer(mRendererModel.activeRenderer());
}

QueueModel::~QueueModel() = default;

int QueueModel::rowCount(QModelIndex const& parent) const noexcept
{
    Q_UNUSED(parent);
    return static_cast<int>(mQueue.entries().size());
}

QHash<int, QByteArray> QueueModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        {static_cast<int>(DisplayRole::Title), QByteArray{"title"}},
        {static_cast<int>(DisplayRole::Artist), QByteArray{"artist"}},
        {static_cast<int>(DisplayRole::ArtworkUrl), QByteArray{"artworkUrl"}},
        {static_cast<int>(DisplayRole::Current), QByteArray{"current"}},
    };
    return roles;
}

QVariant QueueModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or index.row() >= mQueue.entries().size()) {
        qCCritical(shell) << "Failed to request data. Error: invalid index:" << index.row()
                          << "Queue size:" << mQueue.entries().size();
        return {};
    }

    auto const& entry = mQueue.entries().at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::Title) {
        return entry.mainText();
    } else if (dispRole == DisplayRole::Artist) {
        return entry.secondaryText();
    } else if (dispRole == DisplayRole::ArtworkUrl) {
        return entry.artworkUrl();
    } else if (dispRole == DisplayRole::Current) {
        return mQueue.currentIndex() == index.row();
    }
    return {};
}

bool QueueModel::isRunning() const noexcept
{
    return mQueue.state() == Queue::State::Running;
}

void QueueModel::replace(Multimedia::Items const& playables, qsizetype startIndex) noexcept
{
    mQueue.replace(playables, startIndex);
}

void QueueModel::onCurrentEntryChanged() noexcept
{
    auto const previousRow = mCurrentRow;
    mCurrentRow = mQueue.currentIndex();
    notifyCurrentChanged(previousRow);
    notifyCurrentChanged(mCurrentRow);
}

void QueueModel::notifyCurrentChanged(std::optional<qsizetype> row) noexcept
{
    if (row.has_value()) {
        auto const idx = index(static_cast<int>(row.value()));
        Q_EMIT dataChanged(idx, idx, {static_cast<int>(DisplayRole::Current)});
    }
}

} // namespace Shell
