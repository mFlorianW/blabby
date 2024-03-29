// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractInteractiveControl.hpp"
#include <QDebug>

namespace Shell
{

AbstractInteractiveControl::AbstractInteractiveControl()
    : QQuickItem()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

AbstractInteractiveControl::~AbstractInteractiveControl() = default;

bool AbstractInteractiveControl::isHovered() const noexcept
{
    return mHoverState;
}

bool AbstractInteractiveControl::isPressed() const noexcept
{
    return mPressState;
}

void AbstractInteractiveControl::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mPressState = true;
        Q_EMIT pressedChanged();
    }
}

void AbstractInteractiveControl::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mPressState = false;
        Q_EMIT pressedChanged();
        Q_EMIT clicked();
    }
}

void AbstractInteractiveControl::hoverEnterEvent(QHoverEvent* event)
{
    Q_UNUSED(event);
    mHoverState = true;
    Q_EMIT hoveredChanged();
}

void AbstractInteractiveControl::hoverLeaveEvent(QHoverEvent* event)
{
    Q_UNUSED(event);
    mHoverState = false;
    Q_EMIT hoveredChanged();
}

} // namespace Shell
