// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestSource.hpp"
#include "LoggingCategories.hpp"
#include <QDebug>

namespace Multimedia::TestHelper
{

TestSource::TestSource(QString name, QString iconUrl)
    : Multimedia::Source{std::move(name), std::move(iconUrl)}
{
    mItems.insert(QStringLiteral("0"),
                  {Multimedia::Item{Multimedia::ItemType::Playable,
                                    QStringLiteral("MediaItem1"),
                                    QStringLiteral("Artist1"),
                                    QStringLiteral("http://localhost/art1.jpg")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem2")},
                   Multimedia::Item{Multimedia::ItemType::Container,
                                    QStringLiteral("Container1"),
                                    QString(""),
                                    QString(""),
                                    QStringLiteral("1")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem4")}});
    mItems.insert(QStringLiteral("1"),
                  {Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem3")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem4")},
                   Multimedia::Item{Multimedia::ItemType::Container,
                                    QStringLiteral("Container2"),
                                    QString(""),
                                    QString(""),
                                    QStringLiteral("2")}});
    mItems.insert(QStringLiteral("2"),
                  {Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem5")},
                   Multimedia::Item{Multimedia::ItemType::Playable, QStringLiteral("MediaItem6")}});
    navigateTo(QStringLiteral("0"));
}

TestSource::~TestSource() = default;

void TestSource::navigateTo(QString const& path) noexcept
{
    mLastNavigationPath = path;
    ++mNavigationCount;
    if (mHoldNavigations) {
        mPendingPath = path;
        return;
    }
    finishNavigation(path);
}

void TestSource::finishNavigation(QString const& path) noexcept
{
    if (mItems.contains(path)) {
        mMediaItems = mItems[path];
    } else {
        qCCritical(testMediaSource) << "Path not found. Error: Invalied Path" << path << "passed";
    }
    Q_EMIT navigationFinished(path);
}

QString const& TestSource::lastNavigatedPath() const noexcept
{
    return mLastNavigationPath;
}

qsizetype TestSource::navigationCount() const noexcept
{
    return mNavigationCount;
}

void TestSource::setHoldNavigations(bool hold) noexcept
{
    mHoldNavigations = hold;
}

void TestSource::finishPendingNavigation() noexcept
{
    finishNavigation(mPendingPath);
}

void TestSource::failPendingNavigation() noexcept
{
    Q_EMIT navigationFailed(mPendingPath);
}

} // namespace Multimedia::TestHelper
