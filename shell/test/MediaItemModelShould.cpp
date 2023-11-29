// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaItemModelShould.hpp"
#include "MediaItemModel.hpp"
#include "TestMediaSource.hpp"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>

namespace Shell
{

MediaItemModelShould::~MediaItemModelShould() = default;

void MediaItemModelShould::give_correct_amount_of_items()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    auto const count = miModel.rowCount({});

    QCOMPARE(count, 5);
}

void MediaItemModelShould::give_the_correct_display_roles()
{
    auto miModel = MediaItemModel{};
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto const expRoles = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle), QByteArray{"mediaItemTitle"})};

    auto const roles = miModel.roleNames();

    QCOMPARE(expRoles.size(), roles.size());
    QCOMPARE(expRoles, roles);
}

void MediaItemModelShould::give_the_correct_title_for_valid_index_data()
{
    QTest::addColumn<int>("itemIdx");
    QTest::addColumn<QString>("expectedTitle");

    QTest::newRow("Item Index 0") << 0 << QStringLiteral("MediaItem1");
    QTest::newRow("Item Index 1") << 1 << QStringLiteral("MediaItem2");
    QTest::newRow("Item Index 2") << 2 << QStringLiteral("Container1");
    QTest::newRow("Item Index 3") << 3 << QStringLiteral("MediaItem3");
    QTest::newRow("Item Index 4") << 4 << QStringLiteral("MediaItem4");
}

void MediaItemModelShould::give_the_correct_title_for_valid_index()
{
    QFETCH(int, itemIdx);
    QFETCH(QString, expectedTitle);

    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    auto const title =
        miModel.data(miModel.index(itemIdx), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();

    QCOMPARE(title, expectedTitle);
}

void MediaItemModelShould::navigate_when_a_container_item_is_activated()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    miModel.activateMediaItem(2);

    QCOMPARE(mediaSrc->lastNavigationPath(), QStringLiteral("1"));
}

void MediaItemModelShould::update_the_media_items_when_navigation_is_finished()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto modelAboutToReset = QSignalSpy{&miModel, &MediaItemModel::modelAboutToBeReset};
    auto modelReset = QSignalSpy{&miModel, &MediaItemModel::modelReset};

    miModel.activateMediaItem(2);
    QCOMPARE(modelAboutToReset.size(), 1);
    QCOMPARE(modelReset.size(), 1);
    QCOMPARE(miModel.rowCount({}), 2);
    auto const title0 =
        miModel.data(miModel.index(0), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();
    auto const title1 =
        miModel.data(miModel.index(1), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();

    QCOMPARE(title0, QStringLiteral("MediaItem3"));
    QCOMPARE(title1, QStringLiteral("MediaItem4"));
}

void MediaItemModelShould::emit_playRequest_when_a_playable_item_is_activated()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto playRequestSpy = QSignalSpy{&miModel, &MediaItemModel::playRequest};

    miModel.activateMediaItem(1);

    QCOMPARE(playRequestSpy.size(), 1);
    QCOMPARE(playRequestSpy.at(0).at(0).value<Multimedia::MediaItem>().mainText(), QStringLiteral("MediaItem2"));
}

} // namespace Shell

QTEST_MAIN(Shell::MediaItemModelShould)
