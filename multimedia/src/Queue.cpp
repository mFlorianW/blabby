// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Queue.hpp"
#include "private/LoggingCategories.hpp"
#include <QRandomGenerator>
#include <QSet>
#include <algorithm>

namespace Multimedia
{

namespace
{
/**
 * A Current Entry counts as finished when the last known position was at most this far from its end.
 */
constexpr auto endWindow = std::chrono::seconds{3};

/**
 * Previous restarts a Current Entry that played for longer than this, it plays the preceding entry otherwise.
 */
constexpr auto restartThreshold = std::chrono::seconds{3};
} // namespace

struct Queue::Collection
{
    /**
     * A Container on the way down from the collected Container, with the page of its Items that is walked through.
     */
    struct Level
    {
        QString mPath;
        Items mPage;
        // The index of the next Item of the page to walk through.
        qsizetype mPageIndex{0};
        // The index of the first Item of the next page.
        qsizetype mNextStartIndex{0};
        bool mComplete{false};
    };

    std::weak_ptr<Source> mSource;
    QMetaObject::Connection mSourceDestroyed;
    Item mContainer;
    Placement mPlacement{Placement::End};
    // The collected Container first, the Container whose Items are walked through last.
    QList<Level> mLevels;
    // The paths of the Containers walked through, a Container referenced inside itself is only collected once.
    QSet<QString> mVisitedPaths;
    Items mPlayables;
    std::unique_ptr<PendingPage> mPage;
};

Queue::Queue()
    : Queue{[](qsizetype bound) {
        return static_cast<qsizetype>(QRandomGenerator::global()->bounded(static_cast<quint64>(bound)));
    }}
{
}

Queue::Queue(RandomSource randomSource)
    : mRandomSource{std::move(randomSource)}
{
}

Queue::~Queue() = default;

void Queue::setActiveRenderer(std::shared_ptr<Renderer> renderer) noexcept
{
    if (mRenderer == renderer) {
        return;
    }

    auto const handsOver = mState == State::Running and mCurrentIndex.has_value() and renderer != nullptr and
                           renderer->availability() == Renderer::Availability::Online;
    if (mRenderer != nullptr) {
        // disconnect only the own connections, others keep observing the Renderer.
        disconnect(mRenderer.get(), nullptr, this, nullptr);
        mRenderer->setPositionTracked(false);
        // The music must not play in two rooms, the previous Renderer is stopped also when another controller has it
        // or the Current Entry is still loading. A Renderer the Queue never played on is left to its controller.
        if (handsOver and not mHandOverPending and mRenderer->availability() == Renderer::Availability::Online) {
            mRenderer->stop();
        }
    }

    mRenderer = std::move(renderer);
    cancelPendingHandOver();
    if (mRenderer == nullptr) {
        setState(State::Idle);
        return;
    }

    connect(mRenderer.get(), &Renderer::stateChanged, this, &Queue::onRendererStateChanged);
    connect(mRenderer.get(), &Renderer::currentTrackChanged, this, &Queue::updatePlaysCurrentEntry);
    // Not recorded on a change of the Current Track: the device reports a new track before its position is polled,
    // the Renderer still gives the position and the duration of the previous track until then.
    connect(mRenderer.get(), &Renderer::positionChanged, this, &Queue::recordPosition);
    connect(mRenderer.get(), &Renderer::durationChanged, this, &Queue::recordPosition);
    connect(mRenderer.get(), &Renderer::availabilityChanged, this, &Queue::onAvailabilityChanged);
    connect(mRenderer.get(), &Renderer::initializationFinished, this, &Queue::onInitializationFinished);
    connect(mRenderer.get(), &Renderer::initializationFailed, this, &Queue::onInitializationFailed);
    // The protocols of the Renderer are needed to play, the position to tell when the Current Entry finished.
    mRenderer->initialize();
    mRenderer->setPositionTracked(true);
    if (handsOver) {
        // The Current Entry is played once the protocols of the new Renderer are known.
        mHandOverPending = true;
    } else {
        setState(State::Idle);
    }
    recordPosition();
    updatePlaysCurrentEntry();
}

std::shared_ptr<Renderer> const& Queue::activeRenderer() const noexcept
{
    return mRenderer;
}

Items const& Queue::entries() const noexcept
{
    return mEntries;
}

std::optional<qsizetype> Queue::currentIndex() const noexcept
{
    return mCurrentIndex;
}

std::optional<Item> Queue::currentEntry() const noexcept
{
    if (not mCurrentIndex.has_value()) {
        return std::nullopt;
    }
    return mEntries.at(*mCurrentIndex);
}

std::chrono::milliseconds Queue::lastKnownPosition() const noexcept
{
    return mLastKnownPosition;
}

Queue::State Queue::state() const noexcept
{
    return mState;
}

bool Queue::playsCurrentEntry() const noexcept
{
    return mPlaysCurrentEntry;
}

Queue::Repeat Queue::repeat() const noexcept
{
    return mRepeat;
}

void Queue::setRepeat(Queue::Repeat repeat) noexcept
{
    if (mRepeat != repeat) {
        mRepeat = repeat;
        Q_EMIT repeatChanged();
    }
}

bool Queue::isShuffled() const noexcept
{
    return mShuffled;
}

void Queue::setShuffled(bool shuffled) noexcept
{
    if (mShuffled == shuffled) {
        return;
    }

    mShuffled = shuffled;
    if (mShuffled) {
        shuffleAll();
    } else {
        mPlayOrder.clear();
    }
    Q_EMIT shuffledChanged();
}

void Queue::shuffleAll() noexcept
{
    mPlayOrder.clear();
    auto others = QList<qsizetype>{};
    for (auto index = qsizetype{0}; index < mEntries.size(); ++index) {
        if (index == mCurrentIndex) {
            mPlayOrder.append(index);
        } else {
            others.append(index);
        }
    }
    shuffleIn(others, mPlayOrder.size());
}

void Queue::shuffleIn(QList<qsizetype> const& indices, qsizetype first) noexcept
{
    // Each entry goes to a random position among the entries that weren't played yet, from the first position on.
    for (auto const index : indices) {
        mPlayOrder.insert(first + mRandomSource(mPlayOrder.size() - first + 1), index);
    }
}

qsizetype Queue::playPosition() const noexcept
{
    return mCurrentIndex.has_value() ? mPlayOrder.indexOf(*mCurrentIndex) : qsizetype{-1};
}

void Queue::replace(Items const& playables, qsizetype startIndex) noexcept
{
    if (not playables.isEmpty() and (startIndex < 0 or startIndex >= playables.size())) {
        qCWarning(mmQueue) << "Failed to replace the Queue. Error: invalid start index" << startIndex << "for"
                           << playables.size() << "Playables";
        return;
    }

    setShuffled(false);
    reset(playables, startIndex);
}

void Queue::reset(Items const& playables, qsizetype startIndex) noexcept
{
    auto const hadCurrentEntry = mCurrentIndex.has_value();
    Q_EMIT entriesAboutToBeReplaced();
    mEntries = playables;
    // The previous Current Entry is gone with the replaced entries, the new one is notified afterwards.
    mCurrentIndex.reset();
    if (mShuffled) {
        shuffleAll();
    }
    Q_EMIT entriesReplaced();

    if (mEntries.isEmpty()) {
        setLastKnownPosition(std::chrono::milliseconds{0});
        if (hadCurrentEntry) {
            Q_EMIT currentEntryChanged();
        }
    } else {
        setCurrentIndex(startIndex);
    }
    startCurrentEntry();
}

void Queue::play(qsizetype index) noexcept
{
    if (index < 0 or index >= mEntries.size()) {
        qCWarning(mmQueue) << "Failed to play an entry of the Queue. Error: invalid index" << index << "for"
                           << mEntries.size() << "entries";
        return;
    }

    if (mShuffled and mCurrentIndex.has_value() and index != *mCurrentIndex) {
        // The picked entry plays now, the entries that weren't played yet stay ahead of it.
        mPlayOrder.removeOne(index);
        mPlayOrder.insert(playPosition() + 1, index);
    }
    playAt(index);
}

void Queue::playAt(qsizetype index) noexcept
{
    setCurrentIndex(index);
    startCurrentEntry();
}

void Queue::togglePlayback() noexcept
{
    if (mRenderer == nullptr or mRenderer->isPlaybackControlPending() or mRenderer->isTransitioning()) {
        return;
    }

    if (mRenderer->state() == Renderer::State::Playing) {
        cancelPendingHandOver();
        mRenderer->stop();
        setState(State::Idle);
        return;
    }

    if (mHandOverPending) {
        return;
    }

    if (mState == State::Idle and mCurrentIndex.has_value() and not isInControl()) {
        loadCurrentEntryAtLastKnownPosition();
        setState(State::Running);
        return;
    }

    mRenderer->resume();
    if (isInControl()) {
        setState(State::Running);
    }
}

bool Queue::hasPrevious() const noexcept
{
    return mCurrentIndex.has_value();
}

bool Queue::hasNext() const noexcept
{
    return nextIndex().has_value();
}

void Queue::previous() noexcept
{
    if (not mCurrentIndex.has_value()) {
        return;
    }

    auto const index = previousIndex();
    if (mLastKnownPosition > restartThreshold or not index.has_value()) {
        restartCurrentEntry();
    } else {
        playAt(*index);
    }
}

void Queue::next() noexcept
{
    if (auto const index = nextIndex(); index.has_value()) {
        playAt(*index);
    }
}

std::optional<qsizetype> Queue::nextIndex() const noexcept
{
    if (not mCurrentIndex.has_value()) {
        return std::nullopt;
    }
    auto const position = mShuffled ? playPosition() : *mCurrentIndex;
    if (position + 1 < mEntries.size()) {
        return mShuffled ? mPlayOrder.at(position + 1) : position + 1;
    }
    // The Queue starts over after its last entry.
    if (mRepeat == Repeat::All) {
        return mShuffled ? mPlayOrder.first() : 0;
    }
    return std::nullopt;
}

std::optional<qsizetype> Queue::previousIndex() const noexcept
{
    if (not mCurrentIndex.has_value()) {
        return std::nullopt;
    }
    auto const position = mShuffled ? playPosition() : *mCurrentIndex;
    if (position == 0) {
        return std::nullopt;
    }
    return mShuffled ? mPlayOrder.at(position - 1) : position - 1;
}

void Queue::restartCurrentEntry() noexcept
{
    if (not mCurrentIndex.has_value()) {
        return;
    }

    auto const currentIndex = *mCurrentIndex;
    // Only a Renderer that plays the Current Entry can seek in it, otherwise it plays something else or nothing.
    if (isInControl() and mRenderer->state() == Renderer::State::Playing and mRenderer->canSeek()) {
        mRenderer->seek(std::chrono::milliseconds{0});
        setLastKnownPosition(std::chrono::milliseconds{0});
        setState(State::Running);
    } else {
        playAt(currentIndex);
    }
}

void Queue::append(Items const& playables) noexcept
{
    insert(mEntries.size(), playables, Placement::End);
}

void Queue::playNext(Items const& playables) noexcept
{
    insert(mCurrentIndex.has_value() ? *mCurrentIndex + 1 : mEntries.size(), playables, Placement::Next);
}

void Queue::insert(qsizetype index, Items const& playables, Placement placement) noexcept
{
    if (playables.isEmpty()) {
        return;
    }

    auto const hadCurrentEntry = mCurrentIndex.has_value();
    Q_EMIT entriesAboutToBeInserted(index, index + playables.size() - 1);
    mEntries = mEntries.first(index) + playables + mEntries.sliced(index);
    if (mCurrentIndex.has_value() and *mCurrentIndex >= index) {
        mCurrentIndex = *mCurrentIndex + playables.size();
    }
    if (mShuffled) {
        for (auto& entryIndex : mPlayOrder) {
            if (entryIndex >= index) {
                entryIndex += playables.size();
            }
        }
        auto inserted = QList<qsizetype>{};
        for (auto entryIndex = index; entryIndex < index + playables.size(); ++entryIndex) {
            inserted.append(entryIndex);
        }
        if (not hadCurrentEntry) {
            // The first inserted Playable becomes the Current Entry after the entries that were already played, all
            // others weren't played yet.
            mPlayOrder.append(inserted.takeFirst());
            shuffleIn(inserted, mPlayOrder.size());
        } else if (placement == Placement::Next) {
            // Play next means next, also while shuffled.
            auto const position = playPosition();
            for (auto offset = qsizetype{0}; offset < inserted.size(); ++offset) {
                mPlayOrder.insert(position + 1 + offset, inserted.at(offset));
            }
        } else {
            shuffleIn(inserted, playPosition() + 1);
        }
    }
    Q_EMIT entriesInserted();

    if (not hadCurrentEntry) {
        // The Queue stays as it is, Idle, and starts with the first added Playable.
        setCurrentIndex(index);
    }
}

void Queue::append(std::shared_ptr<Source> const& source, Item const& item) noexcept
{
    add(source, item, Placement::End);
}

void Queue::playNext(std::shared_ptr<Source> const& source, Item const& item) noexcept
{
    add(source, item, Placement::Next);
}

bool Queue::isCollecting() const noexcept
{
    return mCollection != nullptr;
}

std::optional<Item> Queue::collectedContainer() const noexcept
{
    if (mCollection == nullptr) {
        return std::nullopt;
    }
    return mCollection->mContainer;
}

void Queue::cancelCollection() noexcept
{
    endCollection();
}

void Queue::place(Items const& playables, Placement placement) noexcept
{
    if (placement == Placement::Next) {
        playNext(playables);
    } else {
        append(playables);
    }
}

void Queue::add(std::shared_ptr<Source> const& source, Item const& item, Placement placement) noexcept
{
    if (item.type() == ItemType::Playable) {
        place(Items{item}, placement);
        return;
    }

    if (source == nullptr) {
        qCWarning(mmQueue) << "Failed to collect the Container" << item.mainText() << "Error: no Source";
        return;
    }

    if (mCollection != nullptr) {
        // Only one collection runs at a time, the new one cancels the running one.
        disconnect(mCollection->mSourceDestroyed);
    }
    mCollection = std::make_unique<Collection>();
    mCollection->mSource = source;
    mCollection->mContainer = item;
    mCollection->mPlacement = placement;
    mCollection->mLevels.append(Collection::Level{.mPath = item.path(), .mPage = {}});
    mCollection->mVisitedPaths.insert(item.path());
    // The Source is gone when all its holders dropped it, the Queue doesn't keep it alive.
    mCollection->mSourceDestroyed = connect(source.get(), &QObject::destroyed, this, &Queue::endCollection);
    Q_EMIT collectingChanged();
    continueCollection();
}

void Queue::continueCollection() noexcept
{
    // Walks depth-first through the Containers: a Container is collected completely before the Items after it.
    while (mCollection != nullptr) {
        if (mCollection->mLevels.isEmpty()) {
            auto const collection = std::exchange(mCollection, nullptr);
            disconnect(collection->mSourceDestroyed);
            place(collection->mPlayables, collection->mPlacement);
            Q_EMIT collectingChanged();
            return;
        }

        auto& level = mCollection->mLevels.last();
        if (level.mPageIndex < level.mPage.size()) {
            auto const& item = level.mPage.at(level.mPageIndex++);
            if (item.type() == ItemType::Playable) {
                mCollection->mPlayables.append(item);
            } else if (not mCollection->mVisitedPaths.contains(item.path())) {
                mCollection->mVisitedPaths.insert(item.path());
                mCollection->mLevels.append(Collection::Level{.mPath = item.path(), .mPage = {}});
            }
            continue;
        }

        if (level.mComplete) {
            mCollection->mLevels.removeLast();
            continue;
        }

        auto const source = mCollection->mSource.lock();
        if (source == nullptr) {
            endCollection();
            return;
        }
        mCollection->mPage = source->browsePage(level.mPath, level.mNextStartIndex);
        if (not mCollection->mPage->isFinished()) {
            connect(mCollection->mPage.get(), &PendingPage::finished, this, &Queue::onPageFinished);
            return;
        }
        if (not takePage()) {
            return;
        }
    }
}

void Queue::onPageFinished() noexcept
{
    if (takePage()) {
        continueCollection();
    }
}

bool Queue::takePage() noexcept
{
    if (mCollection == nullptr or mCollection->mPage == nullptr) {
        return false;
    }

    // The page may be emitting its finished signal, so it's deleted later.
    auto* const page = mCollection->mPage.release();
    page->deleteLater();
    if (page->hasFailed()) {
        auto const container = mCollection->mContainer;
        qCWarning(mmQueue) << "Failed to collect the Container" << container.mainText()
                           << "Error: a page couldn't be loaded";
        endCollection();
        Q_EMIT collectionFailed(container);
        return false;
    }

    auto& level = mCollection->mLevels.last();
    level.mPage = page->items();
    level.mPageIndex = 0;
    level.mNextStartIndex += page->items().size();
    // A page without Items ends the Container, also when the Container shrank meanwhile.
    level.mComplete = page->items().isEmpty() or level.mNextStartIndex >= page->totalItemCount();
    return true;
}

void Queue::endCollection() noexcept
{
    if (mCollection == nullptr) {
        return;
    }

    disconnect(mCollection->mSourceDestroyed);
    mCollection.reset();
    Q_EMIT collectingChanged();
}

void Queue::remove(qsizetype index) noexcept
{
    if (index < 0 or index >= mEntries.size()) {
        qCWarning(mmQueue) << "Failed to remove an entry of the Queue. Error: invalid index" << index << "for"
                           << mEntries.size() << "entries";
        return;
    }

    auto const removesCurrentEntry = mCurrentIndex == index;
    auto const stopRenderer = removesCurrentEntry and isRendererOnCurrentEntry();
    // The entry that follows the removed Current Entry, by its index after the remove.
    auto following = removesCurrentEntry and index < mEntries.size() - 1 ? std::optional{index} : std::nullopt;
    Q_EMIT entryAboutToBeRemoved(index);
    mEntries.removeAt(index);
    if (mShuffled) {
        auto const position = mPlayOrder.indexOf(index);
        mPlayOrder.removeAt(position);
        for (auto& entryIndex : mPlayOrder) {
            if (entryIndex > index) {
                --entryIndex;
            }
        }
        following = removesCurrentEntry and position < mPlayOrder.size() ? std::optional{mPlayOrder.at(position)}
                                                                         : std::nullopt;
    }
    if (removesCurrentEntry) {
        // The index must not point behind the entries meanwhile, the new Current Entry is notified afterwards.
        mCurrentIndex.reset();
    } else if (mCurrentIndex.has_value() and *mCurrentIndex > index) {
        mCurrentIndex = *mCurrentIndex - 1;
    }
    Q_EMIT entryRemoved();

    if (not removesCurrentEntry) {
        return;
    }

    if (following.has_value()) {
        setCurrentIndex(following);
        if (mState == State::Running) {
            startCurrentEntry();
        }
        return;
    }

    mLastKnownDuration.reset();
    setLastKnownPosition(std::chrono::milliseconds{0});
    Q_EMIT currentEntryChanged();
    if (stopRenderer) {
        mRenderer->stop();
    }
    setState(State::Idle);
}

void Queue::move(qsizetype from, qsizetype to) noexcept
{
    if (from < 0 or from >= mEntries.size() or to < 0 or to >= mEntries.size()) {
        qCWarning(mmQueue) << "Failed to move an entry of the Queue. Error: invalid index" << from << "or" << to
                           << "for" << mEntries.size() << "entries";
        return;
    }
    if (from == to) {
        return;
    }

    Q_EMIT entryAboutToBeMoved(from, to);
    mEntries.move(from, to);
    auto const remap = [from, to](qsizetype& entryIndex) {
        if (entryIndex == from) {
            entryIndex = to;
        } else if (from < entryIndex and to >= entryIndex) {
            --entryIndex;
        } else if (from > entryIndex and to <= entryIndex) {
            ++entryIndex;
        }
    };
    if (mCurrentIndex.has_value()) {
        remap(*mCurrentIndex);
    }
    // The play order stays, only the indices of its entries change.
    std::ranges::for_each(mPlayOrder, remap);
    Q_EMIT entryMoved();
}

void Queue::clear() noexcept
{
    auto const stopRenderer = isRendererOnCurrentEntry();
    // Shuffle is kept, unlike for a replace.
    reset({}, 0);
    if (stopRenderer) {
        mRenderer->stop();
    }
}

Queue::Snapshot Queue::snapshot() const noexcept
{
    return Snapshot{
        .entries = mEntries,
        .currentIndex = mCurrentIndex,
        .lastKnownPosition = mLastKnownPosition,
        .state = mState,
        .playsCurrentEntry = mPlaysCurrentEntry,
        .playOrder = mPlayOrder,
    };
}

void Queue::restore(Queue::Snapshot const& snapshot) noexcept
{
    auto const validIndex = not snapshot.currentIndex.has_value() or
                            (*snapshot.currentIndex >= 0 and *snapshot.currentIndex < snapshot.entries.size());
    if (not validIndex) {
        qCWarning(mmQueue) << "Failed to restore the Queue. Error: invalid Current Entry index"
                           << *snapshot.currentIndex << "for" << snapshot.entries.size() << "entries";
        return;
    }

    Q_EMIT entriesAboutToBeReplaced();
    mEntries = snapshot.entries;
    mCurrentIndex = snapshot.currentIndex;
    if (mShuffled) {
        if (snapshot.playOrder.size() == mEntries.size()) {
            mPlayOrder = snapshot.playOrder;
        } else {
            shuffleAll();
        }
    }
    Q_EMIT entriesReplaced();
    Q_EMIT currentEntryChanged();

    mLastKnownDuration.reset();
    auto const stillPlays = isInControl() and mRenderer->state() == Renderer::State::Playing;
    if (snapshot.playsCurrentEntry and not stillPlays) {
        setLastKnownPosition(std::chrono::milliseconds{0});
        startCurrentEntry();
        return;
    }

    setLastKnownPosition(snapshot.lastKnownPosition);
    // The Renderer may have moved on meanwhile, e.g. while the Current Entry was removed.
    recordPosition();
    setState(mRenderer != nullptr ? snapshot.state : State::Idle);
}

bool Queue::isRendererOnCurrentEntry() const noexcept
{
    return isInControl() and
           (mRenderer->state() == Renderer::State::Playing or mRenderer->state() == Renderer::State::Paused);
}

bool Queue::isInControl() const noexcept
{
    return mRenderer != nullptr and mCurrentIndex.has_value() and
           mRenderer->currentTrack().uri == mEntries.at(*mCurrentIndex).playUrl();
}

void Queue::recordPosition() noexcept
{
    // The last known position is kept until the Current Entry is continued on the Renderer.
    if (not isInControl() or mHandOverPending) {
        return;
    }
    if (mPendingSeek.has_value()) {
        seekToPendingPosition();
        return;
    }

    mLastKnownDuration = mRenderer->duration();
    setLastKnownPosition(mRenderer->position());
}

void Queue::onRendererStateChanged() noexcept
{
    updatePlaysCurrentEntry();
    if (not mCurrentIndex.has_value() or mState != State::Running or mRenderer->state() != Renderer::State::Stopped or
        not isInControl()) {
        return;
    }

    // A Current Entry without a duration, e.g. a stream, never finishes on its own.
    auto const finished = mLastKnownDuration.has_value() and *mLastKnownDuration - mLastKnownPosition <= endWindow;
    if (not finished) {
        return;
    }

    auto const index = mRepeat == Repeat::One ? mCurrentIndex : nextIndex();
    if (not index.has_value()) {
        setState(State::Idle);
        return;
    }

    // The Current Entry played again starts from its beginning, too.
    setCurrentIndex(index);
    playCurrentEntry();
}

void Queue::onAvailabilityChanged() noexcept
{
    if (mRenderer->availability() == Renderer::Availability::Offline) {
        cancelPendingHandOver();
        setState(State::Idle);
    }
}

void Queue::onInitializationFinished() noexcept
{
    if (mHandOverPending) {
        loadCurrentEntryAtLastKnownPosition();
    }
}

void Queue::onInitializationFailed() noexcept
{
    if (mHandOverPending) {
        cancelPendingHandOver();
        setState(State::Idle);
    }
}

void Queue::cancelPendingHandOver() noexcept
{
    mHandOverPending = false;
    mPendingSeek.reset();
}

void Queue::seekToPendingPosition() noexcept
{
    // The Renderer can tell whether it seeks in the Current Entry only once it plays it, a position info without a
    // duration, e.g. of a stream, can't be seeked in.
    if (not mPendingSeek.has_value() or mRenderer->state() != Renderer::State::Playing) {
        return;
    }

    auto const position = *mPendingSeek;
    mPendingSeek.reset();
    if (mRenderer->canSeek()) {
        mRenderer->seek(position);
        mLastKnownDuration = mRenderer->duration();
        setLastKnownPosition(position);
    } else {
        recordPosition();
    }
}

void Queue::playCurrentEntry() noexcept
{
    cancelPendingHandOver();
    if (mCurrentIndex.has_value()) {
        mRenderer->playback(mEntries.at(*mCurrentIndex));
    }
}

void Queue::loadCurrentEntryAtLastKnownPosition() noexcept
{
    auto const position = mLastKnownPosition;
    playCurrentEntry();
    if (position > std::chrono::milliseconds{0}) {
        mPendingSeek = position;
    }
}

void Queue::startCurrentEntry() noexcept
{
    if (mCurrentIndex.has_value() and mRenderer != nullptr) {
        playCurrentEntry();
        setState(State::Running);
    } else {
        setState(State::Idle);
    }
    // The Current Entry may have changed without a notification, e.g. by a replace.
    updatePlaysCurrentEntry();
}

void Queue::updatePlaysCurrentEntry() noexcept
{
    auto const plays = mState == State::Running and isInControl() and mRenderer->state() == Renderer::State::Playing;
    if (mPlaysCurrentEntry != plays) {
        mPlaysCurrentEntry = plays;
        Q_EMIT playsCurrentEntryChanged();
    }
}

void Queue::setCurrentIndex(std::optional<qsizetype> index) noexcept
{
    // A new Current Entry hasn't been seen on the Renderer yet.
    mLastKnownDuration.reset();
    setLastKnownPosition(std::chrono::milliseconds{0});
    if (mCurrentIndex != index) {
        mCurrentIndex = index;
        Q_EMIT currentEntryChanged();
    }
    updatePlaysCurrentEntry();
}

void Queue::setLastKnownPosition(std::chrono::milliseconds position) noexcept
{
    if (mLastKnownPosition != position) {
        mLastKnownPosition = position;
        Q_EMIT lastKnownPositionChanged();
    }
}

void Queue::setState(Queue::State state) noexcept
{
    if (mState != state) {
        mState = state;
        Q_EMIT stateChanged();
    }
    updatePlaysCurrentEntry();
}

} // namespace Multimedia
