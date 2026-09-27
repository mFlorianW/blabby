// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CLOCK_H
#define CLOCK_H

#include "blabbyupnpav_export.h"
#include <QObject>
#include <QTimer>
#include <chrono>

namespace UPnPAV
{

/**
 * The Clock provides the current time and wakes its user up at a requested point in time.
 * It exists so that time dependent behaviour, e.g. the expiry of SSDP announcements, can be tested
 * without waiting.
 */
class BLABBYUPNPAV_EXPORT Clock : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Clock)
public:
    using TimePoint = std::chrono::steady_clock::time_point;

    ~Clock() override;

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
    Clock() = default;
};

/**
 * The SteadyClock is the @ref Clock that uses the monotonic system clock.
 */
class BLABBYUPNPAV_EXPORT SteadyClock final : public Clock
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(SteadyClock)
public:
    SteadyClock();
    ~SteadyClock() override;

    TimePoint now() const noexcept override;
    void wakeUpAt(TimePoint timePoint) noexcept override;

private:
    QTimer mTimer;
};

} // namespace UPnPAV

#endif // CLOCK_H
