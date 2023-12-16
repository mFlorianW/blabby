// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "Source.hpp"
#include "private/NavigationStack.hpp"

namespace Multimedia
{

struct SourcePrivate
{
    SourcePrivate(QString sourceName, QString iconUrl, Source &ms)
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

Source::~Source() = default;

Source::Source(QString sourceName, QString iconUrl)
    : d{std::make_unique<SourcePrivate>(std::move(sourceName), std::move(iconUrl), *this)}
{
}

const QString &Source::sourceName() const noexcept
{
    return d->mSourceName;
}

QString const &Source::iconUrl() const noexcept
{
    return d->mIconUrl;
}

Items const &Source::mediaItems() const noexcept
{
    return mMediaItems;
}

void Source::navigateTo(QString const &path) noexcept
{
    Q_UNUSED(path)
}

void Source::navigateBack() noexcept
{
    d->mNavigationStack.navigateBack();
}

void Source::navigateForward() noexcept
{
    d->mNavigationStack.navigateForward();
}

} // namespace Multimedia
