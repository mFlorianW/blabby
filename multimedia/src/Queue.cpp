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
