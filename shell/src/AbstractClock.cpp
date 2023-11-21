// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractClock.hpp"

namespace Shell
{

AbstractClock::AbstractClock()
    : QObject()
{
    connect(&m_timer, &QTimer::timeout, this, &AbstractClock::currentTimeChanged);

    setupTimerInterval();
}

AbstractClock::~AbstractClock() = default;

QTime AbstractClock::getCurrentTime()
{
    setupTimerInterval();
    return QTime::currentTime();
}

void AbstractClock::setupTimerInterval()
{
    const qint32 msTillNextMinute = 60000 - (QTime::currentTime().msecsSinceStartOfDay() % 60000);
    m_timer.start(msTillNextMinute);
}

} // namespace Shell
