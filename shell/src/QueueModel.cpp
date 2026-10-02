// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QueueModel.hpp"
#include "LoggingCategories.hpp"
#include <algorithm>

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
        Q_EMIT summaryChanged();
        Q_EMIT stepAvailabilityChanged();
    });
    connect(&mQueue, &Queue::entriesAboutToBeInserted, this, [this](qsizetype first, qsizetype last) {
        beginInsertRows(QModelIndex{}, static_cast<int>(first), static_cast<int>(last));
    });
    connect(&mQueue, &Queue::entriesInserted, this, [this] {
        // Restoring the Queue before the last remove or clear would drop the added Playables.
        mUndoSnapshot.reset();
        // The Current Entry stays but may have moved down, a new one is notified by the Queue afterwards.
        mCurrentRow = mQueue.currentIndex();
        endInsertRows();
        Q_EMIT summaryChanged();
        Q_EMIT stepAvailabilityChanged();
    });
    connect(&mQueue, &Queue::entryAboutToBeRemoved, this, [this](qsizetype index) {
        beginRemoveRows(QModelIndex{}, static_cast<int>(index), static_cast<int>(index));
    });
    connect(&mQueue, &Queue::entryRemoved, this, [this] {
        // A removed Current Entry is notified by the Queue afterwards, the others may have only moved up.
        mCurrentRow = mQueue.currentIndex();
        endRemoveRows();
        Q_EMIT summaryChanged();
        Q_EMIT stepAvailabilityChanged();
    });
    connect(&mQueue, &Queue::entryAboutToBeMoved, this, [this](qsizetype from, qsizetype to) {
        // The destination of a move is the row the entry is placed before, it's behind the target row when moving down.
        auto const destination = to > from ? to + 1 : to;
        beginMoveRows(QModelIndex{},
                      static_cast<int>(from),
                      static_cast<int>(from),
                      QModelIndex{},
                      static_cast<int>(destination));
    });
    connect(&mQueue, &Queue::entryMoved, this, [this] {
        mCurrentRow = mQueue.currentIndex();
        endMoveRows();
        Q_EMIT stepAvailabilityChanged();
    });
    connect(&mQueue, &Queue::currentEntryChanged, this, &QueueModel::onCurrentEntryChanged);
    connect(&mQueue, &Queue::stateChanged, this, &QueueModel::runningChanged);
    connect(&mQueue, &Queue::playsCurrentEntryChanged, this, &QueueModel::currentEntryPlayingChanged);
    connect(&mQueue, &Queue::collectingChanged, this, &QueueModel::collectingChanged);
    connect(&mQueue, &Queue::collectionFailed, this, [this](Item const& container) {
        Q_EMIT collectionFailed(container.mainText());
    });
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
        {static_cast<int>(DisplayRole::Album), QByteArray{"album"}},
        {static_cast<int>(DisplayRole::Duration), QByteArray{"duration"}},
        {static_cast<int>(DisplayRole::HasDuration), QByteArray{"hasDuration"}},
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
        // The same fallback as for the Current Track of a Renderer.
        return entry.mainText().isEmpty() ? titleOfUri(entry.playUrl()) : entry.mainText();
    } else if (dispRole == DisplayRole::Artist) {
        return entry.secondaryText();
    } else if (dispRole == DisplayRole::ArtworkUrl) {
        return entry.artworkUrl();
    } else if (dispRole == DisplayRole::Current) {
        return mQueue.currentIndex() == index.row();
    } else if (dispRole == DisplayRole::Album) {
        return entry.album();
    } else if (dispRole == DisplayRole::Duration) {
        return static_cast<qreal>(entry.duration().value_or(std::chrono::milliseconds{0}).count());
    } else if (dispRole == DisplayRole::HasDuration) {
        return entry.duration().has_value();
    }
    return {};
}

bool QueueModel::isRunning() const noexcept
{
    return mQueue.state() == Queue::State::Running;
}

bool QueueModel::isCurrentEntryPlaying() const noexcept
{
    return mQueue.playsCurrentEntry();
}

qreal QueueModel::totalDuration() const noexcept
{
    auto total = std::chrono::milliseconds{0};
    for (auto const& entry : mQueue.entries()) {
        total += entry.duration().value_or(std::chrono::milliseconds{0});
    }
    return static_cast<qreal>(total.count());
}

bool QueueModel::hasTotalDuration() const noexcept
{
    return std::ranges::any_of(mQueue.entries(), [](auto const& entry) {
        return entry.duration().has_value();
    });
}

bool QueueModel::isTotalDurationPartial() const noexcept
{
    return std::ranges::any_of(mQueue.entries(), [](auto const& entry) {
        return not entry.duration().has_value();
    });
}

bool QueueModel::hasPrevious() const noexcept
{
    return mQueue.hasPrevious();
}

bool QueueModel::hasNext() const noexcept
{
    return mQueue.hasNext();
}

bool QueueModel::isCollecting() const noexcept
{
    return mQueue.isCollecting();
}

QString QueueModel::collectedContainerTitle() const noexcept
{
    auto const container = mQueue.collectedContainer();
    return container.has_value() ? container->mainText() : QString{};
}

void QueueModel::playNext(std::shared_ptr<Multimedia::Source> const& source, Multimedia::Item const& item) noexcept
{
    mQueue.playNext(source, item);
}

void QueueModel::addToQueue(std::shared_ptr<Multimedia::Source> const& source, Multimedia::Item const& item) noexcept
{
    mQueue.append(source, item);
}

void QueueModel::cancelCollection() noexcept
{
    mQueue.cancelCollection();
}

void QueueModel::replace(Multimedia::Items const& playables, qsizetype startIndex) noexcept
{
    // A replaced Queue isn't the one a remove or a clear changed anymore.
    mUndoSnapshot.reset();
    mQueue.replace(playables, startIndex);
}

void QueueModel::play(int row) noexcept
{
    mQueue.play(row);
}

void QueueModel::appendAndPlay(Multimedia::Item const& playable) noexcept
{
    mQueue.append({playable});
    mQueue.play(mQueue.entries().size() - 1);
}

void QueueModel::remove(int row) noexcept
{
    mUndoSnapshot = mQueue.snapshot();
    mQueue.remove(row);
}

void QueueModel::move(int from, int to) noexcept
{
    mQueue.move(from, to);
}

void QueueModel::clear() noexcept
{
    mUndoSnapshot = mQueue.snapshot();
    mQueue.clear();
}

void QueueModel::undo() noexcept
{
    if (not mUndoSnapshot.has_value()) {
        return;
    }

    mQueue.restore(*mUndoSnapshot);
    mUndoSnapshot.reset();
}

void QueueModel::previous() noexcept
{
    mQueue.previous();
}

void QueueModel::next() noexcept
{
    mQueue.next();
}

void QueueModel::onCurrentEntryChanged() noexcept
{
    auto const previousRow = mCurrentRow;
    mCurrentRow = mQueue.currentIndex();
    notifyCurrentChanged(previousRow);
    notifyCurrentChanged(mCurrentRow);
    Q_EMIT stepAvailabilityChanged();
}

void QueueModel::notifyCurrentChanged(std::optional<qsizetype> row) noexcept
{
    if (row.has_value()) {
        auto const idx = index(static_cast<int>(row.value()));
        Q_EMIT dataChanged(idx, idx, {static_cast<int>(DisplayRole::Current)});
    }
}

} // namespace Shell
