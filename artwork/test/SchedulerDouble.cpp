// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SchedulerDouble.hpp"

namespace Artwork
{

SchedulerDouble::SchedulerDouble() = default;
SchedulerDouble::~SchedulerDouble() = default;

Scheduler::TimePoint SchedulerDouble::now() const noexcept
{
    return mNow;
}

void SchedulerDouble::wakeUpAt(TimePoint timePoint) noexcept
{
    mWakeUpAt = timePoint;
}

void SchedulerDouble::advance(std::chrono::steady_clock::duration duration)
{
    mNow += duration;
    if (mWakeUpAt.has_value() and mWakeUpAt.value() <= mNow) {
        mWakeUpAt.reset();
        Q_EMIT wokeUp();
    }
}

} // namespace Artwork
