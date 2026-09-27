// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Clock.hpp"
#include <algorithm>
#include <limits>

namespace UPnPAV
{

Clock::~Clock() = default;

SteadyClock::SteadyClock()
{
    mTimer.setSingleShot(true);
    mTimer.setTimerType(Qt::VeryCoarseTimer);
    (void)connect(&mTimer, &QTimer::timeout, this, &Clock::wokeUp);
}

SteadyClock::~SteadyClock() = default;

Clock::TimePoint SteadyClock::now() const noexcept
{
    return std::chrono::steady_clock::now();
}

void SteadyClock::wakeUpAt(TimePoint timePoint) noexcept
{
    // QTimer intervals are limited to int milliseconds. A later point in time is reached by waking up
    // early, the user of the clock then requests the next wake up.
    constexpr auto maxInterval = std::chrono::milliseconds{std::numeric_limits<int>::max()};
    auto const remaining = std::chrono::ceil<std::chrono::milliseconds>(timePoint - now());
    mTimer.start(std::clamp(remaining, std::chrono::milliseconds{0}, maxInterval));
}

} // namespace UPnPAV
