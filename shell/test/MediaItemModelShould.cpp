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
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle), QByteArray{"mediaItemTitle"}),
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemIconUrl), QByteArray{"mediaItemIconUrl"}),
    };

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

    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("1"));
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
    QCOMPARE(miModel.rowCount({}), 3);
    auto const title0 =
        miModel.data(miModel.index(0), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();
    auto const title1 =
        miModel.data(miModel.index(1), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();
    auto const title2 =
        miModel.data(miModel.index(2), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();

    QCOMPARE(title0, QStringLiteral("MediaItem3"));
    QCOMPARE(title1, QStringLiteral("MediaItem4"));
    QCOMPARE(title2, QStringLiteral("Container2"));
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

void MediaItemModelShould::navigate_the_back_the_active_media_source()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    miModel.activateMediaItem(2);
    QCOMPARE(miModel.rowCount({}), 3);

    miModel.navigateBack();
    QCOMPARE(miModel.rowCount({}), 5);
}

void MediaItemModelShould::give_the_default_icon_url_when_the_media_item_has_no_icon()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestMediaSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    // default icon URL for playable item
    auto const containerUrl =
        miModel.data(miModel.index(2), static_cast<int>(MediaItemModel::DisplayRole::MediaItemIconUrl)).toString();
    QCOMPARE(containerUrl, QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/24x24/folder.svg"));
    // default icon URL for container item
    auto const iconUrl =
        miModel.data(miModel.index(0), static_cast<int>(MediaItemModel::DisplayRole::MediaItemIconUrl)).toString();

    QCOMPARE(iconUrl, QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/24x24/play_arrow.svg"));
}

void MediaItemModelShould::give_the_name_and_icon_url_for_the_active_media_source()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc =
        std::make_shared<Multimedia::TestHelper::TestMediaSource>(QStringLiteral("MediaSource"),
                                                                  QStringLiteral("http:/localhost/1234.png"));
    auto mediaSourceChangedSpy = QSignalSpy{&miModel, &MediaItemModel::mediaSourceChanged};
    miModel.setMediaSource(mediaSrc);

    QCOMPARE(mediaSourceChangedSpy.size(), 1);
    QCOMPARE(miModel.property("mediaSourceName").toString(), QStringLiteral("MediaSource"));
    QCOMPARE(miModel.property("mediaSourceIconUrl").toString(), QStringLiteral("http:/localhost/1234.png"));
}

} // namespace Shell

QTEST_MAIN(Shell::MediaItemModelShould)
