// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Queue.hpp"
#include "private/LoggingCategories.hpp"

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

Queue::Queue() = default;

Queue::~Queue() = default;

void Queue::setActiveRenderer(std::shared_ptr<Renderer> renderer) noexcept
{
    if (mRenderer == renderer) {
        return;
    }

    if (mRenderer != nullptr) {
        // disconnect only the own connections, others keep observing the Renderer.
        disconnect(mRenderer.get(), nullptr, this, nullptr);
        mRenderer->setPositionTracked(false);
    }

    mRenderer = std::move(renderer);
    if (mRenderer == nullptr) {
        updatePlaysCurrentEntry();
        return;
    }

    connect(mRenderer.get(), &Renderer::stateChanged, this, &Queue::onRendererStateChanged);
    connect(mRenderer.get(), &Renderer::currentTrackChanged, this, &Queue::updatePlaysCurrentEntry);
    // Not recorded on a change of the Current Track: the device reports a new track before its position is polled,
    // the Renderer still gives the position and the duration of the previous track until then.
    connect(mRenderer.get(), &Renderer::positionChanged, this, &Queue::recordPosition);
    connect(mRenderer.get(), &Renderer::durationChanged, this, &Queue::recordPosition);
    // The protocols of the Renderer are needed to play, the position to tell when the Current Entry finished.
    mRenderer->initialize();
    mRenderer->setPositionTracked(true);
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

void Queue::replace(Items const& playables, qsizetype startIndex) noexcept
{
    if (not playables.isEmpty() and (startIndex < 0 or startIndex >= playables.size())) {
        qCWarning(mmQueue) << "Failed to replace the Queue. Error: invalid start index" << startIndex << "for"
                           << playables.size() << "Playables";
        return;
    }

    auto const hadCurrentEntry = mCurrentIndex.has_value();
    Q_EMIT entriesAboutToBeReplaced();
    mEntries = playables;
    // The previous Current Entry is gone with the replaced entries, the new one is notified afterwards.
    mCurrentIndex.reset();
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

    setCurrentIndex(index);
    startCurrentEntry();
}

bool Queue::hasPrevious() const noexcept
{
    return mCurrentIndex.has_value();
}

bool Queue::hasNext() const noexcept
{
    return mCurrentIndex.has_value() and *mCurrentIndex + 1 < mEntries.size();
}

void Queue::previous() noexcept
{
    if (not mCurrentIndex.has_value()) {
        return;
    }

    auto const currentIndex = *mCurrentIndex;
    if (mLastKnownPosition > restartThreshold or currentIndex == 0) {
        restartCurrentEntry();
    } else {
        play(currentIndex - 1);
    }
}

void Queue::next() noexcept
{
    if (mCurrentIndex.has_value() and *mCurrentIndex + 1 < mEntries.size()) {
        play(*mCurrentIndex + 1);
    }
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
        play(currentIndex);
    }
}

void Queue::append(Items const& playables) noexcept
{
    if (playables.isEmpty()) {
        return;
    }

    Q_EMIT entriesAboutToBeAppended(mEntries.size(), mEntries.size() + playables.size() - 1);
    mEntries.append(playables);
    Q_EMIT entriesAppended();
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
    Q_EMIT entryAboutToBeRemoved(index);
    mEntries.removeAt(index);
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

    if (index < mEntries.size()) {
        setCurrentIndex(index);
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
    if (mCurrentIndex.has_value()) {
        auto& current = *mCurrentIndex;
        if (current == from) {
            current = to;
        } else if (from < current and to >= current) {
            --current;
        } else if (from > current and to <= current) {
            ++current;
        }
    }
    Q_EMIT entryMoved();
}

void Queue::clear() noexcept
{
    auto const stopRenderer = isRendererOnCurrentEntry();
    replace({}, 0);
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
    if (not isInControl()) {
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

    auto const nextIndex = *mCurrentIndex + 1;
    if (nextIndex >= mEntries.size()) {
        setState(State::Idle);
        return;
    }

    setCurrentIndex(nextIndex);
    playCurrentEntry();
}

void Queue::playCurrentEntry() noexcept
{
    if (mCurrentIndex.has_value()) {
        mRenderer->playback(mEntries.at(*mCurrentIndex));
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
