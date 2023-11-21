// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItemShould.hpp"
#include "MediaItem.hpp"
#include <QTest>

namespace Multimedia
{

MediaItemShould::~MediaItemShould() = default;

void MediaItemShould::give_the_type()
{
    auto const mItem = MediaItem{MediaItemType::Container, QString{""}};
    auto type = mItem.type();
    QVERIFY2(type == MediaItemType::Container,
             QString("The MediaItemTpye \"%1\" is not the expected one %2")
                 .arg(QString::number(static_cast<qint32>(type)),
                      QString::number(static_cast<qint32>(MediaItemType::Container)))
                 .toLocal8Bit());

    auto const mItem2 = MediaItem{MediaItemType::Playable, QString{""}};
    type = mItem2.type();
    QVERIFY2(type == MediaItemType::Playable,
             QString("The MediaItemTpye \"%1\" is not the expected one %2")
                 .arg(QString::number(static_cast<qint32>(type)),
                      QString::number(static_cast<qint32>(MediaItemType::Playable)))
                 .toLocal8Bit());
}

void MediaItemShould::give_the_main_and_secondary_text()
{
    const auto expMainText = QStringLiteral("Hello");
    const auto expSecText = QStringLiteral("World");
    auto const mItem = MediaItem{MediaItemType::Container, expMainText, expSecText};

    auto const &mText = mItem.mainText();
    auto const &secondaryText = mItem.secondaryText();

    QVERIFY2(mText == expMainText,
             QString("The main text \"%1\" is not the expected one %2").arg(mText, expMainText).toLocal8Bit());
    QVERIFY2(secondaryText == expSecText,
             QString("The secondaryText text \"%1\" is not the expected one %2").arg(mText, expSecText).toLocal8Bit());
}

void MediaItemShould::give_the_icon_url()
{
    const auto expIconUrl = QStringLiteral("http://exmaple.com/Blubdi.png");
    auto const mItem = MediaItem{MediaItemType::Container, QString{""}, QString{""}, expIconUrl};

    const auto iconUrl = mItem.iconUrl();

    QVERIFY2(iconUrl == expIconUrl,
             QString("The icon URL \"%1\" is not the expected one %2").arg(iconUrl, expIconUrl).toLocal8Bit());
}

void MediaItemShould::give_the_path()
{
    const auto expPath = QStringLiteral("1234");
    auto const mItem = MediaItem{MediaItemType::Container, QString{""}, QString{""}, QString{""}, expPath};

    const auto path = mItem.path();

    QVERIFY2(path == expPath, QString("The path \"%1\" is not the expected one %2").arg(path, expPath).toLocal8Bit());
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::MediaItemShould)
