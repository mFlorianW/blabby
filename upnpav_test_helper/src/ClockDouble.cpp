// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ClockDouble.hpp"

namespace UPnPAV
{

ClockDouble::ClockDouble() = default;
ClockDouble::~ClockDouble() = default;

Clock::TimePoint ClockDouble::now() const noexcept
{
    return mNow;
}

void ClockDouble::wakeUpAt(TimePoint timePoint) noexcept
{
    mWakeUpAt = timePoint;
}

void ClockDouble::advance(std::chrono::steady_clock::duration duration)
{
    mNow += duration;
    if (mWakeUpAt.has_value() and mWakeUpAt.value() <= mNow) {
        mWakeUpAt.reset();
        Q_EMIT wokeUp();
    }
}

} // namespace UPnPAV
