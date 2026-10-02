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

    /**
     * True when previous does something, i.e. there is a Current Entry.
     */
    Q_PROPERTY(bool hasPrevious READ hasPrevious NOTIFY stepAvailabilityChanged)

    /**
     * True when next does something, i.e. the Current Entry isn't the last entry.
     */
    Q_PROPERTY(bool hasNext READ hasNext NOTIFY stepAvailabilityChanged)

    /**
     * True while the Playables of a Container are collected to play them next or add them to the Queue.
     */
    Q_PROPERTY(bool collecting READ isCollecting NOTIFY collectingChanged)

    /**
     * The title of the Container whose Playables are collected, empty while nothing is collected.
     */
    Q_PROPERTY(QString collectedContainerTitle READ collectedContainerTitle NOTIFY collectingChanged)

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
     * Gives whether previous does something.
     * @see Multimedia::Queue::hasPrevious
     * @return True when previous does something.
     */
    bool hasPrevious() const noexcept;

    /**
     * Gives whether next does something.
     * @see Multimedia::Queue::hasNext
     * @return True when next does something.
     */
    bool hasNext() const noexcept;

    /**
     * Gives whether the Playables of a Container are collected.
     * @see Multimedia::Queue::isCollecting
     * @return True while a Container is collected.
     */
    bool isCollecting() const noexcept;

    /**
     * Gives the title of the Container whose Playables are collected.
     * @return The title of the collected Container, empty while nothing is collected.
     */
    QString collectedContainerTitle() const noexcept;

    /**
     * Inserts a Playable, or all Playables in a Container, of the Source right after the Current Entry.
     * @see Multimedia::Queue::playNext
     * @param source The Source of the Item.
     * @param item The Playable or the Container to play next.
     */
    void playNext(std::shared_ptr<Multimedia::Source> const& source, Multimedia::Item const& item) noexcept;

    /**
     * Appends a Playable, or all Playables in a Container, of the Source at the end of the Queue.
     * @see Multimedia::Queue::append
     * @param source The Source of the Item.
     * @param item The Playable or the Container to add.
     */
    void addToQueue(std::shared_ptr<Multimedia::Source> const& source, Multimedia::Item const& item) noexcept;

    /**
     * Cancels the collection of a Container, the Queue stays unchanged.
     * @see Multimedia::Queue::cancelCollection
     */
    Q_INVOKABLE void cancelCollection() noexcept;

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
     * Restarts the Current Entry or plays the preceding entry on the Active Renderer.
     * @see Multimedia::Queue::previous
     */
    Q_INVOKABLE void previous() noexcept;

    /**
     * Plays the entry after the Current Entry on the Active Renderer.
     * @see Multimedia::Queue::next
     */
    Q_INVOKABLE void next() noexcept;

    /**
     * Appends the Playable at the end of the Queue and plays it on the Active Renderer, the entries before it stay.
     * Without an Active Renderer it only becomes the Current Entry.
     * @see Multimedia::Queue::append
     * @param playable The Playable to append and play.
     */
    void appendAndPlay(Multimedia::Item const& playable) noexcept;

    /**
     * Toggles the playback on the Active Renderer through the Queue: pausing or stopping makes the Queue Idle, Play
     * continues an Idle Queue's Current Entry at its last known position when the Renderer isn't on it.
     * @see Multimedia::Queue::togglePlayback
     */
    Q_INVOKABLE void togglePlayback() noexcept;

    /**
     * Removes the entry at the row, it can be undone with @ref Shell::QueueModel::undo.
     * @see Multimedia::Queue::remove
     * @param row The row of the entry to remove.
     */
    Q_INVOKABLE void remove(int row) noexcept;

    /**
     * Moves the entry at a row to another row without interrupting the playback.
     * @see Multimedia::Queue::move
     * @param from The row of the entry to move.
     * @param to The row the entry has afterwards.
     */
    Q_INVOKABLE void move(int from, int to) noexcept;

    /**
     * Removes all entries, it can be undone with @ref Shell::QueueModel::undo.
     * @see Multimedia::Queue::clear
     */
    Q_INVOKABLE void clear() noexcept;

    /**
     * Restores the Queue as it was before the last remove or clear, once.
     * Nothing happens when there is nothing to undo, e.g. after the Queue was replaced or Playables were added.
     * @see Multimedia::Queue::restore
     */
    Q_INVOKABLE void undo() noexcept;

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

    /**
     * This signal is emitted when previous or next may have become available or unavailable.
     */
    void stepAvailabilityChanged();

    /**
     * This signal is emitted when a collection of a Container started or ended.
     */
    void collectingChanged();

    /**
     * This signal is emitted when the Playables of a Container couldn't be collected, the Queue is unchanged.
     * @param containerTitle The title of the Container.
     */
    void collectionFailed(QString const& containerTitle);

private:
    void onCurrentEntryChanged() noexcept;
    void notifyCurrentChanged(std::optional<qsizetype> row) noexcept;

    MediaRendererModel const& mRendererModel;
    Multimedia::Queue mQueue;
    std::optional<qsizetype> mCurrentRow;
    std::optional<Multimedia::Queue::Snapshot> mUndoSnapshot;
};

} // namespace Shell
