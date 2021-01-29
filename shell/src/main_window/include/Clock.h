// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CLOCK_H
#define CLOCK_H

#include <QObject>
#include <QTime>
#include <QTimer>

namespace Shell
{

class Clock : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Clock)

    Q_PROPERTY(QTime currentTime READ getCurrentTime NOTIFY currentTimeChanged)

public:
    /**
     * Crete instance of Clock
     */
    explicit Clock();

    /**
     * @return The system current time in 24hour format.
     */
    QTime getCurrentTime();

Q_SIGNALS:
    /**
     * On every new minute this signal is emitted so
     * the clock is updated.
     */
    void currentTimeChanged();

private:
    /**
     * Setup the timer to update the clock on the
     * next minute.
     */
    void setupTimerInterval();

private:
    QTimer m_timer;
};

} // namespace Shell

#endif // CLOCK_H
