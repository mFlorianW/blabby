// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItem.h"

namespace Multimedia
{

MediaItem::MediaItem(MediaItemType type, QString mainText, QString secondaryText, QString iconUrl)
    : d{new MediaItemData{.mType = type,
                          .mMainText = std::move(mainText),
                          .mSecondaryText = secondaryText,
                          .mIconUrl = std::move(iconUrl)}}
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

} // namespace Multimedia
