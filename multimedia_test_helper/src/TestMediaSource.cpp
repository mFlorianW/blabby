// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "TestMediaSource.hpp"
#include "LoggingCategories.hpp"
#include <QDebug>

namespace Multimedia::TestHelper
{

TestMediaSource::TestMediaSource(QString name, QString iconUrl)
    : Multimedia::MediaSource{std::move(name), std::move(iconUrl)}
{
    mItems.insert(QStringLiteral("0"),
                  {Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem1")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem2")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Container,
                                         QStringLiteral("Container1"),
                                         QString(""),
                                         QString(""),
                                         QStringLiteral("1")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem4")}});
    mItems.insert(QStringLiteral("1"),
                  {Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem4")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Container,
                                         QStringLiteral("Container2"),
                                         QString(""),
                                         QString(""),
                                         QStringLiteral("2")}});
    mItems.insert(QStringLiteral("2"),
                  {Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem5")},
                   Multimedia::MediaItem{Multimedia::MediaItemType::Playable, QStringLiteral("MediaItem6")}});
    navigateTo(QStringLiteral("0"));
}

TestMediaSource::~TestMediaSource() = default;

void TestMediaSource::navigateTo(QString const &path) noexcept
{
    mLastNavigationPath = path;
    if (path == QStringLiteral("0")) {
        mMediaItems = mItems[QStringLiteral("0")];
    } else if (path == QStringLiteral("1")) {
        mMediaItems = mItems[QStringLiteral("1")];
    } else if (path == QStringLiteral("2")) {
        mMediaItems = mItems[QStringLiteral("2")];
    } else {
        qCCritical(testMediaSource) << "Path not found. Error: Invalied Path" << path << "passed";
    }
    Q_EMIT navigationFinished(path);
}

const QString &TestMediaSource::lastNavigatedPath() const noexcept
{
    return mLastNavigationPath;
}

} // namespace Multimedia::TestHelper
