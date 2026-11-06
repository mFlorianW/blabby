// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Renderer.hpp"
#include "Source.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>
#include <chrono>
#include <functional>
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
 * How it advances depends on its Repeat and its Shuffle, which belong to the Queue and are never sent to the Renderer
 * as a play mode. While shuffled the Queue plays its entries in a random play order, the listed order of the entries
 * never changes with it.
 * An entry that the Active Renderer fails to play for the Running Queue, e.g. because it rejects the entry, is skipped:
 * the Queue tells so and plays the following entry, also with the Repeat One, the failed entry stays in the Queue.
 * After three failed entries in a row the Queue tells that it failed and becomes Idle. The Renderer playing the Current
 * Entry and every playback the user starts count the failed entries from zero again.
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
     * How the Queue continues after its Current Entry finished. Repeat belongs to the Queue, not to a Renderer.
     */
    enum class Repeat
    {
        /**
         * The Queue plays the following entry and becomes Idle after the last entry.
         */
        Off,
        /**
         * The Queue plays the following entry and starts over with the first entry after the last entry.
         */
        All,
        /**
         * The Queue plays the Current Entry again.
         */
        One,
    };
    Q_ENUM(Repeat)

    /**
     * Gives a random number in the range [0, bound), bound is at least 1.
     */
    using RandomSource = std::function<qsizetype(qsizetype bound)>;

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
        /**
         * The indices of the entries in the order they play, empty when the Queue wasn't shuffled.
         */
        QList<qsizetype> playOrder;
    };

    /**
     * Creates an empty and Idle Queue without an Active Renderer that shuffles with the global random generator.
     */
    Queue();

    /**
     * Creates an empty and Idle Queue without an Active Renderer.
     * @param randomSource The random source the Queue shuffles with.
     */
    explicit Queue(RandomSource randomSource);

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
     * A Running Queue is handed over: the previous Renderer is stopped, also when another controller took it over, and
     * the new one plays the Current Entry once it's initialized, at the last known position when it can seek and from
     * its start otherwise. A previous Renderer the Queue was still being handed over to is left alone. The Queue
     * becomes Idle when the new Renderer fails to initialize. An Idle Queue isn't handed over, it starts nothing on the
     * new Renderer. Without a new Active Renderer or when it's Offline the Queue becomes Idle. The Queue also becomes
     * Idle when its Active Renderer goes Offline. The Current Entry and its last known position are kept in both cases.
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
     * Gives the Repeat of the Queue.
     * @return The Repeat, Off at start.
     */
    Queue::Repeat repeat() const noexcept;

    /**
     * Sets how the Queue continues after its Current Entry finished. The Repeat is kept when the entries are replaced
     * or restored and on a Hand Over.
     * @param repeat The Repeat.
     */
    void setRepeat(Queue::Repeat repeat) noexcept;

    /**
     * Gives whether the Queue is shuffled.
     * @return True while shuffled, false at start.
     */
    bool isShuffled() const noexcept;

    /**
     * Turns Shuffle on or off, the listed order of the entries and the playback stay.
     * Turning it on keeps the Current Entry and puts all other entries in a random play order after it, previous,
     * next and advancing follow the play order then. Turning it off continues in the listed order after the Current
     * Entry. Shuffle is kept when the entries are cleared or restored and on a Hand Over, it's turned off when the
     * entries are replaced.
     * @param shuffled True to shuffle the Queue.
     */
    void setShuffled(bool shuffled) noexcept;

    /**
     * Replaces the entries with the Playables and plays the start entry on the Active Renderer, the Queue is Running.
     * Shuffle is turned off. Without an Active Renderer the start entry becomes the Current Entry and the Queue is
     * Idle. An empty list of Playables empties the Queue, it's Idle then.
     * @param playables The Playables in the order they shall be played.
     * @param startIndex The index of the Playable that plays first.
     */
    void replace(Items const& playables, qsizetype startIndex) noexcept;

    /**
     * Plays the entry at the index from its start on the Active Renderer, it becomes the Current Entry and the Queue is
     * Running. Without an Active Renderer the entry only becomes the Current Entry and the Queue is Idle.
     * While shuffled the entry is moved in the play order right after the previous Current Entry, so the entries that
     * weren't played yet stay ahead.
     * @param index The index of the entry to play.
     */
    void play(qsizetype index) noexcept;

    /**
     * Appends the Playables at the end of the entries. The Current Entry and the Queue State stay, a Queue without a
     * Current Entry, e.g. an empty one, makes the first appended Playable its Current Entry and stays Idle.
     * While shuffled each Playable is placed at a random position in the play order after the Current Entry.
     * @param playables The Playables in the order they shall be appended.
     */
    void append(Items const& playables) noexcept;

    /**
     * Inserts the Playables right after the Current Entry, so they play next, also while shuffled. The Current Entry
     * and the Queue State stay. A Queue without a Current Entry, e.g. an empty one, appends them and makes the first
     * one its Current Entry, it stays Idle.
     * @param playables The Playables in the order they shall be played.
     */
    void playNext(Items const& playables) noexcept;

    /**
     * Appends a Playable, or all Playables in a Container, of the Source at the end of the entries, like
     * @ref Multimedia::Queue::append(Items const&).
     * A Container is collected first: all Playables inside it, including those in nested Containers, depth-first in
     * Source order and without a cap, page by page through @ref Multimedia::Source::browsePage. The Queue only
     * changes when the collection completes, see @ref Multimedia::Queue::isCollecting.
     * @param source The Source of the Item.
     * @param item The Playable or the Container to append.
     */
    void append(std::shared_ptr<Source> const& source, Item const& item) noexcept;

    /**
     * Inserts a Playable, or all Playables in a Container, of the Source right after the Current Entry, like
     * @ref Multimedia::Queue::playNext(Items const&). They are inserted after the entry that is current when the
     * collection of a Container completes, see @ref Multimedia::Queue::append(std::shared_ptr<Source> const&, Item
     * const&).
     * @param source The Source of the Item.
     * @param item The Playable or the Container to play next.
     */
    void playNext(std::shared_ptr<Source> const& source, Item const& item) noexcept;

    /**
     * Gives whether the Playables of a Container are collected. Only one collection runs at a time, a new one cancels
     * it. It ends when it completes, fails, see @ref Multimedia::Queue::collectionFailed, is cancelled or its Source
     * disappears; the Queue only changes when it completes.
     * @return True while the Playables of a Container are collected.
     */
    bool isCollecting() const noexcept;

    /**
     * Gives the Container whose Playables are collected.
     * @return The collected Container, unset while nothing is collected.
     */
    std::optional<Item> collectedContainer() const noexcept;

    /**
     * Cancels the collection of a Container, the Queue stays unchanged. Does nothing while nothing is collected.
     */
    void cancelCollection() noexcept;

    /**
     * Gives whether there is an entry before the Current Entry or the Current Entry can be restarted.
     * @return True when previous does something, i.e. there is a Current Entry.
     */
    bool hasPrevious() const noexcept;

    /**
     * Gives whether there is an entry after the Current Entry.
     * @return True when next does something, i.e. the Current Entry isn't the last entry or the Repeat is All.
     */
    bool hasNext() const noexcept;

    /**
     * Restarts the Current Entry when it played for more than 3 seconds and plays the preceding entry from its start
     * otherwise, the first entry is always restarted. While shuffled the preceding and the first entry are those of the
     * play order. The Queue is Running afterwards, also when it was Idle or another controller took over the Renderer.
     * A Current Entry that the Active Renderer plays for the Queue is restarted by a seek to its start, it's loaded
     * again when the Renderer can't seek or doesn't play it.
     * Without an Active Renderer the entry only becomes the Current Entry and the Queue is Idle.
     */
    void previous() noexcept;

    /**
     * Plays the entry after the Current Entry from its start, like @ref Multimedia::Queue::play, also with the Repeat
     * One. On the last entry the first entry plays with the Repeat All, nothing happens otherwise. While shuffled the
     * following, the last and the first entry are those of the play order.
     */
    void next() noexcept;

    /**
     * Toggles the playback on the Active Renderer through the Queue.
     * A playing Renderer is paused, or stopped when it can't pause, and the Queue becomes Idle, also when another
     * controller took the Renderer over.
     * Otherwise an Idle Queue whose Current Entry isn't on the Renderer loads it again and continues it at the last
     * known position when the Renderer can seek, the Queue is Running. In all other cases the Renderer resumes what it
     * is on, the Queue is Running afterwards when that is the Current Entry. Play is ignored while a Hand Over is
     * pending, the Hand Over plays the Current Entry.
     * Nothing happens without an Active Renderer or while it's transitioning or a stop or resume is pending.
     */
    void togglePlayback() noexcept;

    /**
     * Removes the entry at the index, the other entries keep their order.
     * Removing an entry that isn't the Current Entry doesn't affect the playback.
     * Removing the Current Entry makes the following entry, in the play order while shuffled, the Current Entry, which
     * plays from its start when the Queue is Running. Without a following entry the Queue is Idle without a Current
     * Entry, and the Active Renderer is stopped when it is on the removed Current Entry.
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
     * Restores the entries, the Current Entry, its last known position and the Queue State of the snapshot. Shuffle
     * stays as it is, a shuffled Queue takes the play order of a shuffled snapshot and shuffles a snapshot that wasn't.
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
     * This signal is emitted before Playables are inserted, e.g. appended.
     * @param first The index the first inserted Playable will have.
     * @param last The index the last inserted Playable will have.
     */
    void entriesAboutToBeInserted(qsizetype first, qsizetype last);

    /**
     * This signal is emitted after Playables are inserted. The Current Entry stays, its index may have changed.
     * A Queue without a Current Entry notifies its new Current Entry afterwards.
     */
    void entriesInserted();

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

    /**
     * This signal is emitted when the Repeat changed, next may have become available or unavailable with it.
     */
    void repeatChanged();

    /**
     * This signal is emitted when Shuffle was turned on or off, next may have become available or unavailable with it.
     */
    void shuffledChanged();

    /**
     * This signal is emitted when a collection of a Container started or ended.
     */
    void collectingChanged();

    /**
     * This signal is emitted when the collection of a Container failed because a page failed, the Queue is unchanged.
     * @param container The Container whose Playables couldn't be collected.
     */
    void collectionFailed(Multimedia::Item const& container);

    /**
     * This signal is emitted when the Active Renderer failed to play an entry for the Running Queue, the Queue plays
     * the following entry, or becomes Idle without one. Not emitted for the failure that makes the Queue fail, see
     * @ref Multimedia::Queue::queueFailed.
     * @param entry The entry that couldn't be played, it stays in the Queue.
     */
    void entryFailed(Multimedia::Item const& entry);

    /**
     * This signal is emitted when the Active Renderer failed to play three entries in a row, the Queue is Idle.
     * @param rendererName The name of the Active Renderer.
     */
    void queueFailed(QString const& rendererName);

private:
    /**
     * Where collected Playables are added.
     */
    enum class Placement
    {
        Next,
        End,
    };

    /**
     * A Container whose Playables are collected, in the order they are found.
     */
    struct Collection;

    void insert(qsizetype index, Items const& playables, Placement placement) noexcept;
    void reset(Items const& playables, qsizetype startIndex) noexcept;
    void shuffleAll() noexcept;
    void shuffleIn(QList<qsizetype> const& indices, qsizetype first) noexcept;
    qsizetype playPosition() const noexcept;
    void playAt(qsizetype index) noexcept;
    void place(Items const& playables, Placement placement) noexcept;
    void add(std::shared_ptr<Source> const& source, Item const& item, Placement placement) noexcept;
    void continueCollection() noexcept;
    void onPageFinished() noexcept;
    bool takePage() noexcept;
    void endCollection() noexcept;

    std::optional<qsizetype> nextIndex() const noexcept;
    std::optional<qsizetype> previousIndex() const noexcept;
    bool isInControl() const noexcept;
    bool isRendererOnCurrentEntry() const noexcept;
    void onRendererStateChanged() noexcept;
    void recordPosition() noexcept;
    void onAvailabilityChanged() noexcept;
    void onInitializationFinished() noexcept;
    void onInitializationFailed() noexcept;
    void onPlaybackFailed(QString const& errorMsg) noexcept;
    void skipFailedEntry(QString const& errorMsg) noexcept;
    void cancelPendingHandOver() noexcept;
    void seekToPendingPosition() noexcept;
    void playCurrentEntry() noexcept;
    void loadCurrentEntryAtLastKnownPosition() noexcept;
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
    Queue::Repeat mRepeat = Queue::Repeat::Off;
    RandomSource mRandomSource;
    bool mShuffled = false;
    // The indices of the entries in the order they play while shuffled, empty otherwise.
    QList<qsizetype> mPlayOrder;
    std::unique_ptr<Collection> mCollection;
    bool mHandOverPending = false;
    std::optional<std::chrono::milliseconds> mPendingSeek;
    // Identifies the latest playback sent to the Active Renderer, a failure is only taken for it.
    quint64 mPlaybackId = 0;
    // The entries in a row the Renderer failed to play.
    qsizetype mFailedEntryCount = 0;
};

} // namespace Multimedia
