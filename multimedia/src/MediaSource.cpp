// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaSource.hpp"
#include "private/NavigationStack.hpp"

namespace Multimedia
{

struct MediaSourcePrivate
{
    MediaSourcePrivate(QString sourceName, QString iconUrl, MediaSource &ms)
        : mSourceName{std::move(sourceName)}
        , mIconUrl{std::move(iconUrl)}
        , mNavigationStack{ms}
    {
    }

    QString mSourceName{""};
    QString mIconUrl{""};
    QString mCurrentPath{""};
    NavigationStack mNavigationStack;
};

MediaSource::~MediaSource() = default;

MediaSource::MediaSource(QString sourceName, QString iconUrl)
    : d{std::make_unique<MediaSourcePrivate>(std::move(sourceName), std::move(iconUrl), *this)}
{
}

const QString &MediaSource::sourceName() const noexcept
{
    return d->mSourceName;
}

QString const &MediaSource::iconUrl() const noexcept
{
    return d->mIconUrl;
}

MediaItems const &MediaSource::mediaItems() const noexcept
{
    return mMediaItems;
}

void MediaSource::navigateTo(QString const &path) noexcept
{
    Q_UNUSED(path)
}

void MediaSource::navigateBack() noexcept
{
    d->mNavigationStack.navigateBack();
}

void MediaSource::navigateForward() noexcept
{
    d->mNavigationStack.navigateForward();
}

} // namespace Multimedia
