// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaSource.h"

namespace Multimedia
{

MediaSource::~MediaSource() = default;

MediaSource::MediaSource(QString sourceName, QString iconUrl)
    : mSourceName{std::move(sourceName)}
    , mIconUrl{std::move(iconUrl)}
{
}

const QString &MediaSource::sourceName() const noexcept
{
    return mSourceName;
}

QString const &MediaSource::iconUrl() const noexcept
{
    return mIconUrl;
}

MediaItems const &MediaSource::mediaItems() const noexcept
{
    return mMediaItems;
}

bool MediaSource::navigateTo(QString const &path) noexcept
{
    Q_UNUSED(path)
    return false;
}

} // namespace Multimedia
