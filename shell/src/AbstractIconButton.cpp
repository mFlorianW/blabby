// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
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

void AbstractIconButton::setSource(QUrl const &source) noexcept
{
    if (mIconSource != source)
    {
        mIconSource = source;
        Q_EMIT sourceChanged();
    }
}

} // namespace Shell
