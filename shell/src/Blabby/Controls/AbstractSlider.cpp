// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractSlider.hpp"

namespace Shell
{

AbstractSlider::AbstractSlider()
{
    connect(this, &AbstractSlider::widthChanged, this, &AbstractSlider::onWidthChanged);
    connect(this, &AbstractSlider::implicitWidthChanged, this, &AbstractSlider::onWidthChanged);
}

AbstractSlider::~AbstractSlider() = default;

qreal AbstractSlider::from() noexcept
{
    return mFrom;
}

void AbstractSlider::setFrom(qreal from) noexcept
{
    if (not qFuzzyCompare(mFrom, from)) {
        mFrom = from;
        Q_EMIT fromChanged();
    }
}

qreal AbstractSlider::to() const noexcept
{
    return mTo;
}

void AbstractSlider::setTo(qreal to) noexcept
{
    if (not qFuzzyCompare(mTo, to)) {
        mTo = to;
        Q_EMIT toChanged();
    }
}

qreal AbstractSlider::value() const noexcept
{
    return mValue;
}

void AbstractSlider::setValue(qreal value) noexcept
{
    if (not qFuzzyCompare(mValue, value)) {
        mValue = value;
        Q_EMIT valueChanged();

        auto const vpos = mValue / mTo * width();
        if (not qFuzzyCompare(mVisualPositon, vpos)) {
            mVisualPositon = vpos;
            Q_EMIT visualPositionChanged();
        }
    }
}

qreal AbstractSlider::visualPosition() const noexcept
{
    return mVisualPositon;
}

void AbstractSlider::handleRelease(QPointF point) noexcept
{
    auto const newValue = mTo / width() * getVisualPosition(point);
    setValue(newValue);
}

void AbstractSlider::onWidthChanged() noexcept
{
    mVisualPositon = mValue / mTo * width();
    Q_EMIT visualPositionChanged();
}

qreal AbstractSlider::getVisualPosition(QPointF point) noexcept
{
    auto releasePoint = point.x() > width() ? width() : point.x();
    releasePoint = releasePoint < 0 ? 0 : releasePoint;
    return releasePoint;
}

} // namespace Shell
