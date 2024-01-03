// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractHeader.hpp"

namespace Shell
{

AbstractHeader::~AbstractHeader() = default;

QString AbstractHeader::text() const noexcept
{
    return mText;
}

void AbstractHeader::setText(QString const &text) noexcept
{
    if (mText != text) {
        mText = text;
        Q_EMIT textChanged();
    }
}

QString AbstractHeader::iconUrl() const noexcept
{
    return mIconUrl;
}

void AbstractHeader::setIconUrl(QString const &iconUrl) noexcept
{
    if (mIconUrl != iconUrl) {
        mIconUrl = iconUrl;
    }
    Q_EMIT iconUrlChanged();
}

} // namespace Shell
