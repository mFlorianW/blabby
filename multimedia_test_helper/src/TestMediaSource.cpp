// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "TestMediaSource.hpp"

namespace Multimedia::TestHelper
{

TestMediaSource::TestMediaSource(QString name, QString iconUrl)
    : Multimedia::MediaSource{std::move(name), std::move(iconUrl)}
{
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem1")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem2")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Container,
                                                   QStringLiteral("Container1"),
                                                   QString(""),
                                                   QString(""),
                                                   QStringLiteral("1")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem3")});
    mMediaItems.emplace_back(Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem4")});
    mItems.insert(QStringLiteral("0"), mMediaItems);
    mItems.insert(QStringLiteral("1"),
                  {Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem4")}});
}

TestMediaSource::~TestMediaSource() = default;

void TestMediaSource::navigateTo(QString const &path) noexcept
{
    mLastNavigationPath = path;
    if (path == QStringLiteral("0"))
    {
        mMediaItems = mItems[QStringLiteral("0")];
    }
    else if (path == QStringLiteral("1"))
    {
        mMediaItems = mItems[QStringLiteral("1")];
    }
    Q_EMIT navigationFinished(path);
}

const QString &TestMediaSource::lastNavigationPath() const noexcept
{
    return mLastNavigationPath;
}

} // namespace Multimedia::TestHelper
