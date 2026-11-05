// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "blabbyartwork_export.h"
#include <QObject>
#include <QTimer>
#include <chrono>

namespace Artwork
{

/**
 * The Scheduler provides the current time and wakes its user up at a requested point in time.
 * It exists so that time dependent behaviour, e.g. the rate limit of an online service, can be tested
 * without waiting.
 */
class BLABBYARTWORK_EXPORT Scheduler : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Scheduler)
public:
    using TimePoint = std::chrono::steady_clock::time_point;

    ~Scheduler() override;

    /**
     * @return The current point in time.
     */
    virtual TimePoint now() const noexcept = 0;

    /**
     * Requests a single @ref wokeUp signal at the given point in time.
     * A new request replaces a previous one that hasn't fired yet.
     * @param timePoint The point in time for the wake up.
     */
    virtual void wakeUpAt(TimePoint timePoint) noexcept = 0;

Q_SIGNALS:
    /**
     * This signal is emitted when the requested wake up point in time is reached.
     */
    void wokeUp();

protected:
    Scheduler() = default;
};

/**
 * The SteadyScheduler is the @ref Scheduler that uses the monotonic system clock.
 */
class BLABBYARTWORK_EXPORT SteadyScheduler final : public Scheduler
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(SteadyScheduler)
public:
    SteadyScheduler();
    ~SteadyScheduler() override;

    TimePoint now() const noexcept override;
    void wakeUpAt(TimePoint timePoint) noexcept override;

private:
    QTimer mTimer;
};

} // namespace Artwork
