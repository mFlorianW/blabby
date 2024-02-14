// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractIconButton.hpp"

namespace Shell
{

AbstractIconButton::~AbstractIconButton() = default;

QUrl AbstractIconButton::source() const noexcept
{
    return mIconSource;
}

void AbstractIconButton::setSource(QUrl const& source) noexcept
{
    if (mIconSource != source) {
        mIconSource = source;
        Q_EMIT sourceChanged();
    }
}

bool AbstractIconButton::border() const noexcept
{
    return mBorder;
}

void AbstractIconButton::setBorder(bool border) noexcept
{
    if (mBorder != border) {
        mBorder = border;
        Q_EMIT borderChanged();
    }
}

AbstractIconButton::Style AbstractIconButton::style() const noexcept
{
    return mStyle;
}

void AbstractIconButton::setStyle(AbstractIconButton::Style style) noexcept
{
    if (mStyle != style) {
        mStyle = style;
        Q_EMIT styleChanged();
    }
}

} // namespace Shell
