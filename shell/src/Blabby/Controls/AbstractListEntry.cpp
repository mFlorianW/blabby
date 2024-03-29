// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "AbstractListEntry.hpp"

namespace Shell
{

AbstractListEntry::~AbstractListEntry() = default;

QString AbstractListEntry::title() const noexcept
{
    return mTitle;
}

void AbstractListEntry::setTitle(QString const& title) noexcept
{
    if (mTitle != title) {
        mTitle = title;
        Q_EMIT titleChanged();
    }
}

QString AbstractListEntry::iconUrl() const noexcept
{
    return mIconUrl;
}

void AbstractListEntry::setIconUrl(QString const& iconUrl) noexcept
{
    if (mIconUrl != iconUrl) {
        mIconUrl = iconUrl;
        Q_EMIT iconUrlChanged();
    }
}

QQuickItem* AbstractListEntry::trailingElement() const noexcept
{
    return mTrailingElement;
}

void AbstractListEntry::setTrailingElement(QQuickItem* trailingElement) noexcept
{
    if (mTrailingElement != trailingElement) {
        mTrailingElement = trailingElement;
        Q_EMIT trailingElementChanged();
    }
}

} // namespace Shell
