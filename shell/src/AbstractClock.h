// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QObject>
#include <QTime>
#include <QTimer>
#include <qqml.h>

namespace Shell
{

class AbstractClock : public QObject
{
    Q_OBJECT

    /**
     * The system current time in 24hour format.
     */
    Q_PROPERTY(QTime currentTime READ getCurrentTime NOTIFY currentTimeChanged)

    QML_ELEMENT

public:
    /**
     * Crete instance of Clock
     */
    explicit AbstractClock();

    /**
     * Destructor
     */
    ~AbstractClock() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(AbstractClock)

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
