// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "ItemShould.hpp"
#include "Item.hpp"
#include <QTest>

namespace Multimedia
{

ItemShould::~ItemShould() = default;

void ItemShould::give_the_type()
{
    auto const item = Item{ItemType::Container, QString{""}};
    auto type = item.type();
    QVERIFY2(
        type == ItemType::Container,
        QString("The MediaItemTpye \"%1\" is not the expected one %2")
            .arg(QString::number(static_cast<qint32>(type)), QString::number(static_cast<qint32>(ItemType::Container)))
            .toLocal8Bit());

    auto const mItem2 = Item{ItemType::Playable, QString{""}};
    type = mItem2.type();
    QVERIFY2(
        type == ItemType::Playable,
        QString("The MediaItemTpye \"%1\" is not the expected one %2")
            .arg(QString::number(static_cast<qint32>(type)), QString::number(static_cast<qint32>(ItemType::Playable)))
            .toLocal8Bit());
}

void ItemShould::give_the_main_and_secondary_text()
{
    const auto expMainText = QStringLiteral("Hello");
    const auto expSecText = QStringLiteral("World");
    auto const item = Item{ItemType::Container, expMainText, expSecText};

    auto const &mText = item.mainText();
    auto const &secondaryText = item.secondaryText();

    QVERIFY2(mText == expMainText,
             QString("The main text \"%1\" is not the expected one %2").arg(mText, expMainText).toLocal8Bit());
    QVERIFY2(secondaryText == expSecText,
             QString("The secondaryText text \"%1\" is not the expected one %2").arg(mText, expSecText).toLocal8Bit());
}

void ItemShould::give_the_icon_url()
{
    const auto expIconUrl = QStringLiteral("http://exmaple.com/Blubdi.png");
    auto const item = Item{ItemType::Container, QString{""}, QString{""}, expIconUrl};

    const auto iconUrl = item.iconUrl();

    QVERIFY2(iconUrl == expIconUrl,
             QString("The icon URL \"%1\" is not the expected one %2").arg(iconUrl, expIconUrl).toLocal8Bit());
}

void ItemShould::give_the_path()
{
    const auto expPath = QStringLiteral("1234");
    auto const item = Item{ItemType::Container, QString{""}, QString{""}, QString{""}, expPath};

    const auto path = item.path();

    QVERIFY2(path == expPath, QString("The path \"%1\" is not the expected one %2").arg(path, expPath).toLocal8Bit());
}

void ItemShould::give_the_play_url()
{
    const auto expUrl = QStringLiteral("http://example.com/123.mp3");
    const auto item = ItemBuilder{}.withPlayUrl(expUrl).build();

    const auto playUrl = item.playUrl();

    QCOMPARE(playUrl, expUrl);
}

void ItemShould::give_supported_types()
{
    const auto expTypes = QStringList(QStringLiteral("http-get:*:audio/mpeg:*"));
    const auto item = ItemBuilder{}.withSupportedTypes(expTypes).build();

    const auto supportedTypes = item.supportedTypes();

    QCOMPARE(supportedTypes, expTypes);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::ItemShould)
