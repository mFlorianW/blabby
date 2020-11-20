#include "Clock.h"

namespace Shell
{

Clock::Clock()
    : QObject()
{
    connect(&m_timer, &QTimer::timeout, this, &Clock::currentTimeChanged);

    setupTimerInterval();
}

QTime Clock::getCurrentTime()
{
    setupTimerInterval();
    return QTime::currentTime();
}

void Clock::setupTimerInterval()
{
    const qint32 msTillNextMinute = 60000 - (QTime::currentTime().msecsSinceStartOfDay() % 60000);
    m_timer.start(msTillNextMinute);
}

} // namespace Shell
