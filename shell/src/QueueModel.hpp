// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererModel.hpp"
#include "Queue.hpp"
#include <QAbstractListModel>

namespace Shell
{

/**
 * A thin list model over Blabby's @ref Multimedia::Queue with one row per entry.
 * It follows the Active Renderer of the @ref Shell::MediaRendererModel and passes it to the Queue.
 */
class QueueModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * True while the Queue is Running, false while it's Idle.
     */
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)

    /**
     * True while the Active Renderer plays the Current Entry for the Running Queue.
     */
    Q_PROPERTY(bool currentEntryPlaying READ isCurrentEntryPlaying NOTIFY currentEntryPlayingChanged)

    /**
     * The number of entries.
     */
    Q_PROPERTY(int entryCount READ rowCount NOTIFY summaryChanged)

    /**
     * The sum of the known durations of the entries in milliseconds.
     */
    Q_PROPERTY(qreal totalDuration READ totalDuration NOTIFY summaryChanged)

    /**
     * True when the duration of at least one entry is known.
     */
    Q_PROPERTY(bool hasTotalDuration READ hasTotalDuration NOTIFY summaryChanged)

    /**
     * True when the duration of at least one entry is unknown, the total duration is then only a lower bound.
     */
    Q_PROPERTY(bool totalDurationPartial READ isTotalDurationPartial NOTIFY summaryChanged)

public:
    /**
     * The roles of an entry.
     */
    enum class DisplayRole
    {
        Title = Qt::UserRole + 1,
        Artist,
        ArtworkUrl,
        Current,
        Album,
        Duration,
        HasDuration,
    };
    Q_ENUM(DisplayRole)

    /**
     * Creates an empty Queue that follows the Active Renderer of the model.
     * @param rendererModel The model whose Active Renderer the Queue plays on, it must outlive the QueueModel.
     */
    explicit QueueModel(MediaRendererModel const& rendererModel);

    /**
     * Default destructor
     */
    ~QueueModel() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(QueueModel)

    /**
     * @copydoc QAbstractItemModel::rowCount(const QModelIndex& parent) const
     */
    int rowCount(QModelIndex const& parent = QModelIndex{}) const noexcept override;

    /**
     * @copydoc QAbstractItemModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * @copydoc QAbstractItemModel::data(const QModelIndex& index, int role) const
     */
    QVariant data(QModelIndex const& index, int role) const noexcept override;

    /**
     * Gives whether the Queue is Running.
     * @return True while the Queue is Running.
     */
    bool isRunning() const noexcept;

    /**
     * Gives whether the Active Renderer plays the Current Entry for the Running Queue.
     * @return True while the Active Renderer plays the Current Entry.
     */
    bool isCurrentEntryPlaying() const noexcept;

    /**
     * Gives the sum of the known durations of the entries.
     * @return The total duration in milliseconds.
     */
    qreal totalDuration() const noexcept;

    /**
     * Gives whether the duration of at least one entry is known.
     * @return True when a duration is known.
     */
    bool hasTotalDuration() const noexcept;

    /**
     * Gives whether the duration of at least one entry is unknown.
     * @return True when a duration is unknown.
     */
    bool isTotalDurationPartial() const noexcept;

    /**
     * Replaces the Queue with the Playables and plays the start entry on the Active Renderer.
     * @see Multimedia::Queue::replace
     * @param playables The Playables in the order they shall be played.
     * @param startIndex The index of the Playable that plays first.
     */
    void replace(Multimedia::Items const& playables, qsizetype startIndex) noexcept;

    /**
     * Plays the entry at the row from its start on the Active Renderer.
     * @see Multimedia::Queue::play
     * @param row The row of the entry to play.
     */
    Q_INVOKABLE void play(int row) noexcept;

    /**
     * Appends the Playable at the end of the Queue and plays it on the Active Renderer, the entries before it stay.
     * Without an Active Renderer it only becomes the Current Entry.
     * @see Multimedia::Queue::append
     * @param playable The Playable to append and play.
     */
    void appendAndPlay(Multimedia::Item const& playable) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the Queue State changed.
     */
    void runningChanged();

    /**
     * This signal is emitted when the Active Renderer started or stopped playing the Current Entry.
     */
    void currentEntryPlayingChanged();

    /**
     * This signal is emitted when the number of entries or the total duration changed.
     */
    void summaryChanged();

private:
    void onCurrentEntryChanged() noexcept;
    void notifyCurrentChanged(std::optional<qsizetype> row) noexcept;

    MediaRendererModel const& mRendererModel;
    Multimedia::Queue mQueue;
    std::optional<qsizetype> mCurrentRow;
};

} // namespace Shell
