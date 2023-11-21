// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItem.hpp"

namespace Multimedia
{

MediaItem::MediaItem(MediaItemType type, QString mainText, QString secondaryText, QString iconUrl, QString path)
    : d{new MediaItemData{type, std::move(mainText), secondaryText, std::move(iconUrl), std::move(path)}}
{
}

MediaItemType MediaItem::type() const noexcept
{
    return d->mType;
}

QString const &MediaItem::mainText() const noexcept
{
    return d->mMainText;
}

QString const &MediaItem::secondaryText() const noexcept
{

    return d->mSecondaryText;
}

QString const &MediaItem::iconUrl() const noexcept
{
    return d->mIconUrl;
}

QString const &MediaItem::path() const noexcept
{
    return d->mPath;
}

bool operator==(MediaItem const &lhs, MediaItem const &rhs) noexcept
{
    // clang-format off
    return (lhs.d == rhs.d) or ((lhs.d->mType == rhs.d->mType) and
                               (lhs.d->mMainText == rhs.d->mMainText) and
                               (lhs.d->mSecondaryText == rhs.d->mSecondaryText) and
                               (lhs.d->mIconUrl == rhs.d->mIconUrl) and
                               (lhs.d->mPath == rhs.d->mPath));
    // clang-format on
}

bool operator!=(MediaItem const &lhs, MediaItem const &rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace Multimedia
