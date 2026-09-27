// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef CLOCKDOUBLE_H
#define CLOCKDOUBLE_H

#include "Clock.hpp"
#include <optional>

namespace UPnPAV
{

/**
 * A @ref Clock whose time only moves when the test advances it.
 */
class ClockDouble final : public Clock
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ClockDouble)
public:
    ClockDouble();
    ~ClockDouble() override;

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

} // namespace UPnPAV

#endif // CLOCKDOUBLE_H
