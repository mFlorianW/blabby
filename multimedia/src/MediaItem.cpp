// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItem.h"

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

} // namespace Multimedia
