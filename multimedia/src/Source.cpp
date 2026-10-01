// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Source.hpp"
#include "private/NavigationStack.hpp"
#include <algorithm>

namespace Multimedia
{

struct SourcePrivate
{
    SourcePrivate(QString sourceName, QString iconUrl, Source& ms)
        : mSourceName{std::move(sourceName)}
        , mIconUrl{std::move(iconUrl)}
        , mNavigationStack{ms}
    {
    }

    QString mSourceName{""};
    QString mIconUrl{""};
    QString mCurrentPath{""};
    qsizetype mTotalItemCount{0};
    NavigationStack mNavigationStack;
};

Source::~Source() = default;

Source::Source(QString sourceName, QString iconUrl)
    : d{std::make_unique<SourcePrivate>(std::move(sourceName), std::move(iconUrl), *this)}
{
}

QString const& Source::sourceName() const noexcept
{
    return d->mSourceName;
}

QString const& Source::iconUrl() const noexcept
{
    return d->mIconUrl;
}

Items const& Source::mediaItems() const noexcept
{
    return mMediaItems;
}

qsizetype Source::totalItemCount() const noexcept
{
    return std::max(d->mTotalItemCount, mMediaItems.size());
}

bool Source::canLoadMore() const noexcept
{
    return mMediaItems.size() < totalItemCount();
}

void Source::navigateTo(QString const& path, qsizetype minimumItemCount) noexcept
{
    navigate(path, minimumItemCount);
}

void Source::navigate(QString const& path, qsizetype minimumItemCount) noexcept
{
    Q_UNUSED(path)
    Q_UNUSED(minimumItemCount)
}

void Source::loadMore() noexcept
{
}

void Source::setTotalItemCount(qsizetype count) noexcept
{
    d->mTotalItemCount = count;
}

void Source::navigateBack(qsizetype minimumItemCount) noexcept
{
    d->mNavigationStack.navigateBack(minimumItemCount);
}

void Source::navigateForward() noexcept
{
    d->mNavigationStack.navigateForward();
}

} // namespace Multimedia
