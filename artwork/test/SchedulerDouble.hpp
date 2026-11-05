// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Scheduler.hpp"
#include <optional>

namespace Artwork
{

/**
 * A @ref Scheduler whose time only moves when the test advances it.
 */
class SchedulerDouble final : public Scheduler
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(SchedulerDouble)
public:
    SchedulerDouble();
    ~SchedulerDouble() override;

    TimePoint now() const noexcept override;
    void wakeUpAt(TimePoint timePoint) noexcept override;

    /**
     * Moves the time forward and emits @ref wokeUp when the requested wake up point is reached.
     * @param duration The amount of time to move forward.
     */
    void advance(std::chrono::steady_clock::duration duration);

private:
    TimePoint mNow;
    std::optional<TimePoint> mWakeUpAt;
};

} // namespace Artwork
