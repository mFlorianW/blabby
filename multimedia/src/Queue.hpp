// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Renderer.hpp"
#include "Source.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>
#include <chrono>
#include <memory>
#include <optional>

namespace Multimedia
{

/**
 * The Queue is the ordered list of Playables that Blabby plays one after another on the Active Renderer.
 * It belongs to Blabby, not to a Renderer, and lives in memory only.
 *
 * The Queue sends one entry at a time to the Active Renderer and advances itself when the Renderer finished the
 * Current Entry (ADR 0003): the Renderer reports Stopped while its Current Track is the Current Entry, matched by URI,
 * and the last known position was within 3 seconds of the duration. Any other Stopped doesn't advance, e.g. a stop on
 * the device, a stream without a duration, or the end of what another controller played.
 */
class BLABBYMULTIMEDIA_EXPORT Queue : public QObject
{
    Q_OBJECT
public:
    /**
     * The Queue State, independent of the Playback State of the Active Renderer.
     */
    enum class State
    {
        /**
         * The Queue doesn't play and doesn't advance.
         */
        Idle,
        /**
         * The Queue plays its Current Entry on the Active Renderer and advances when it finished.
         */
        Running,
    };
    Q_ENUM(State)

    /**
     * What a Queue was at one moment, to give it back to @ref Multimedia::Queue::restore, e.g. to undo a remove.
     */
    struct Snapshot
    {
        Items entries;
        std::optional<qsizetype> currentIndex;
        std::chrono::milliseconds lastKnownPosition{0};
        Queue::State state = Queue::State::Idle;
        bool playsCurrentEntry = false;
    };

    /**
     * Creates an empty and Idle Queue without an Active Renderer.
     */
    Queue();

    /**
     * Default destructor
     */
    ~Queue() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(Queue)

    /**
     * Sets the Active Renderer the Queue plays on and follows, e.g. when the user picked another one.
     * The Queue initializes the Renderer and tracks its position.
     * @param renderer The Active Renderer, nullptr when there is none.
     */
    void setActiveRenderer(std::shared_ptr<Renderer> renderer) noexcept;

    /**
     * Gives the Active Renderer the Queue plays on.
     * @return The Active Renderer, nullptr when there is none.
     */
    std::shared_ptr<Renderer> const& activeRenderer() const noexcept;

    /**
     * Gives the entries in their order.
     * @return The entries.
     */
    Items const& entries() const noexcept;

    /**
     * Gives the index of the Current Entry.
     * @return The index of the Current Entry, unset when there is none.
     */
    std::optional<qsizetype> currentIndex() const noexcept;

    /**
     * Gives the Current Entry.
     * @return The Current Entry, unset when there is none.
     */
    std::optional<Item> currentEntry() const noexcept;

    /**
     * Gives the last position the Queue saw of its Current Entry on the Active Renderer.
     * @return The last known position, 0 when the Current Entry hasn't been played yet.
     */
    std::chrono::milliseconds lastKnownPosition() const noexcept;

    /**
     * Gives the Queue State.
     * @return The Queue State.
     */
    Queue::State state() const noexcept;

    /**
     * Gives whether the Active Renderer plays the Current Entry for the Running Queue, i.e. the Queue is Running, in
     * control of the Renderer and the Renderer is Playing.
     * @return True while the Active Renderer plays the Current Entry for the Queue.
     */
    bool playsCurrentEntry() const noexcept;

    /**
     * Replaces the entries with the Playables and plays the start entry on the Active Renderer, the Queue is Running.
     * Without an Active Renderer the start entry becomes the Current Entry and the Queue is Idle.
     * An empty list of Playables empties the Queue, it's Idle then.
     * @param playables The Playables in the order they shall be played.
     * @param startIndex The index of the Playable that plays first.
     */
    void replace(Items const& playables, qsizetype startIndex) noexcept;

    /**
     * Plays the entry at the index from its start on the Active Renderer, it becomes the Current Entry and the Queue is
     * Running. Without an Active Renderer the entry only becomes the Current Entry and the Queue is Idle.
     * @param index The index of the entry to play.
     */
    void play(qsizetype index) noexcept;

    /**
     * Appends the Playables at the end of the entries. The Current Entry and the Queue State stay, an empty Queue has
     * no Current Entry afterwards either.
     * @param playables The Playables in the order they shall be appended.
     */
    void append(Items const& playables) noexcept;

    /**
     * Gives whether there is an entry before the Current Entry or the Current Entry can be restarted.
     * @return True when previous does something, i.e. there is a Current Entry.
     */
    bool hasPrevious() const noexcept;

    /**
     * Gives whether there is an entry after the Current Entry.
     * @return True when next does something, i.e. the Current Entry isn't the last entry.
     */
    bool hasNext() const noexcept;

    /**
     * Restarts the Current Entry when it played for more than 3 seconds and plays the preceding entry from its start
     * otherwise, the first entry is always restarted. The Queue is Running afterwards, also when it was Idle or
     * another controller took over the Renderer.
     * A Current Entry that the Active Renderer plays for the Queue is restarted by a seek to its start, it's loaded
     * again when the Renderer can't seek or doesn't play it.
     * Without an Active Renderer the entry only becomes the Current Entry and the Queue is Idle.
     */
    void previous() noexcept;

    /**
     * Plays the entry after the Current Entry from its start, like @ref Multimedia::Queue::play.
     * Nothing happens on the last entry.
     */
    void next() noexcept;

    /**
     * Removes the entry at the index, the other entries keep their order.
     * Removing an entry that isn't the Current Entry doesn't affect the playback.
     * Removing the Current Entry makes the following entry the Current Entry, which plays from its start when the
     * Queue is Running. Without a following entry the Queue is Idle without a Current Entry, and the Active Renderer
     * is stopped when it is on the removed Current Entry.
     * @param index The index of the entry to remove.
     */
    void remove(qsizetype index) noexcept;

    /**
     * Moves the entry at an index to another index without interrupting the playback, also the Current Entry.
     * @param from The index of the entry to move.
     * @param to The index the entry has afterwards.
     */
    void move(qsizetype from, qsizetype to) noexcept;

    /**
     * Removes all entries, the Queue is Idle without a Current Entry afterwards.
     * The Active Renderer is stopped when it is on the Current Entry, it's left alone when another controller took
     * it over.
     */
    void clear() noexcept;

    /**
     * Gives what the Queue is now, to restore it later.
     * @return The snapshot of the Queue.
     */
    Queue::Snapshot snapshot() const noexcept;

    /**
     * Restores the entries, the Current Entry, its last known position and the Queue State of the snapshot.
     * A Current Entry that the Active Renderer played for the Queue when the snapshot was taken and doesn't play
     * anymore, e.g. because it was removed, plays again from its start.
     * Without an Active Renderer the restored Queue is Idle.
     * @param snapshot The snapshot to restore.
     */
    void restore(Queue::Snapshot const& snapshot) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted before the entries are replaced.
     */
    void entriesAboutToBeReplaced();

    /**
     * This signal is emitted after the entries are replaced.
     */
    void entriesReplaced();

    /**
     * This signal is emitted before Playables are appended.
     * @param first The index the first appended Playable will have.
     * @param last The index the last appended Playable will have.
     */
    void entriesAboutToBeAppended(qsizetype first, qsizetype last);

    /**
     * This signal is emitted after Playables are appended.
     */
    void entriesAppended();

    /**
     * This signal is emitted before an entry is removed.
     * @param index The index of the entry that will be removed.
     */
    void entryAboutToBeRemoved(qsizetype index);

    /**
     * This signal is emitted after an entry is removed.
     * Only a removed Current Entry changes the Current Entry and is notified afterwards, the index of the Current Entry
     * may have changed nevertheless.
     */
    void entryRemoved();

    /**
     * This signal is emitted before an entry is moved.
     * @param from The index of the entry that will be moved.
     * @param to The index the entry will have.
     */
    void entryAboutToBeMoved(qsizetype from, qsizetype to);

    /**
     * This signal is emitted after an entry is moved. The Current Entry stays, its index may have changed.
     */
    void entryMoved();

    /**
     * This signal is emitted when the Current Entry changed.
     */
    void currentEntryChanged();

    /**
     * This signal is emitted when the last known position of the Current Entry changed.
     */
    void lastKnownPositionChanged();

    /**
     * This signal is emitted when the Queue State changed.
     */
    void stateChanged();

    /**
     * This signal is emitted when the Active Renderer started or stopped playing the Current Entry for the Queue.
     */
    void playsCurrentEntryChanged();

private:
    bool isInControl() const noexcept;
    bool isRendererOnCurrentEntry() const noexcept;
    void onRendererStateChanged() noexcept;
    void recordPosition() noexcept;
    void playCurrentEntry() noexcept;
    void restartCurrentEntry() noexcept;
    void startCurrentEntry() noexcept;
    void updatePlaysCurrentEntry() noexcept;
    void setCurrentIndex(std::optional<qsizetype> index) noexcept;
    void setLastKnownPosition(std::chrono::milliseconds position) noexcept;
    void setState(Queue::State state) noexcept;

    std::shared_ptr<Renderer> mRenderer;
    Items mEntries;
    std::optional<qsizetype> mCurrentIndex;
    std::chrono::milliseconds mLastKnownPosition{0};
    std::optional<std::chrono::milliseconds> mLastKnownDuration;
    Queue::State mState = Queue::State::Idle;
    bool mPlaysCurrentEntry = false;
};

} // namespace Multimedia
