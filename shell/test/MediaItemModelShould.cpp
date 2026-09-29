// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaItemModelShould.hpp"
#include "MediaItemModel.hpp"
#include "TestSource.hpp"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>

namespace Shell
{

MediaItemModelShould::~MediaItemModelShould() = default;

void MediaItemModelShould::give_correct_amount_of_items()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
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
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemArtworkUrl),
                       QByteArray{"mediaItemArtworkUrl"}),
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemSecondaryText),
                       QByteArray{"mediaItemSecondaryText"}),
        std::make_pair(static_cast<int>(MediaItemModel::DisplayRole::MediaItemType), QByteArray{"mediaItemType"}),
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
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    auto const title =
        miModel.data(miModel.index(itemIdx), static_cast<int>(MediaItemModel::DisplayRole::MediaItemTitle)).toString();

    QCOMPARE(title, expectedTitle);
}

void MediaItemModelShould::navigate_when_a_container_item_is_activated()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    miModel.activateMediaItem(2);

    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("1"));
}

void MediaItemModelShould::update_the_media_items_when_navigation_is_finished()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
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

void MediaItemModelShould::do_nothing_when_a_playable_item_is_activated()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto modelResetSpy = QSignalSpy{&miModel, &MediaItemModel::modelReset};

    miModel.activateMediaItem(1);

    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("0"));
    QCOMPARE(modelResetSpy.size(), 0);
    QCOMPARE(miModel.rowCount({}), 5);
}

void MediaItemModelShould::give_the_item_type_of_the_item()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto const typeRole = static_cast<int>(MediaItemModel::DisplayRole::MediaItemType);

    QCOMPARE(miModel.data(miModel.index(1), typeRole).toInt(), static_cast<int>(Multimedia::ItemType::Playable));
    QCOMPARE(miModel.data(miModel.index(2), typeRole).toInt(), static_cast<int>(Multimedia::ItemType::Container));
}

void MediaItemModelShould::tell_whether_a_media_source_is_set()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    QCOMPARE(miModel.property("hasMediaSource").toBool(), false);

    miModel.setMediaSource(mediaSrc);
    QCOMPARE(miModel.property("hasMediaSource").toBool(), true);

    miModel.setMediaSource(nullptr);
    QCOMPARE(miModel.property("hasMediaSource").toBool(), false);
}

void MediaItemModelShould::stop_following_the_previous_media_source_when_the_media_source_changes()
{
    auto miModel = MediaItemModel{};
    auto previousSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(previousSrc);
    miModel.setMediaSource(mediaSrc);
    auto modelResetSpy = QSignalSpy{&miModel, &MediaItemModel::modelReset};

    previousSrc->navigateTo(QStringLiteral("1"));
    QCOMPARE(modelResetSpy.size(), 0);

    mediaSrc->navigateTo(QStringLiteral("1"));
    QCOMPARE(modelResetSpy.size(), 1);
    QCOMPARE(miModel.rowCount({}), 3);
}

void MediaItemModelShould::navigate_the_back_the_active_media_source()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);

    miModel.activateMediaItem(2);
    QCOMPARE(miModel.rowCount({}), 3);

    miModel.navigateBack();
    QCOMPARE(miModel.rowCount({}), 5);
}

void MediaItemModelShould::give_the_artwork_url_and_the_secondary_text_of_the_item()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto const artworkUrlRole = static_cast<int>(MediaItemModel::DisplayRole::MediaItemArtworkUrl);
    auto const secondaryTextRole = static_cast<int>(MediaItemModel::DisplayRole::MediaItemSecondaryText);

    QCOMPARE(miModel.data(miModel.index(0), artworkUrlRole).toString(), QStringLiteral("http://localhost/art1.jpg"));
    QCOMPARE(miModel.data(miModel.index(0), secondaryTextRole).toString(), QStringLiteral("Artist1"));
    // Items without artwork and secondary text give empty values, the view shows its placeholder.
    QCOMPARE(miModel.data(miModel.index(1), artworkUrlRole).toString(), QString{});
    QCOMPARE(miModel.data(miModel.index(1), secondaryTextRole).toString(), QString{});
}

void MediaItemModelShould::give_the_name_and_icon_url_for_the_active_media_source()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QStringLiteral("MediaSource"),
                                                                         QStringLiteral("http:/localhost/1234.png"));
    auto mediaSourceChangedSpy = QSignalSpy{&miModel, &MediaItemModel::mediaSourceChanged};
    miModel.setMediaSource(mediaSrc);

    QCOMPARE(mediaSourceChangedSpy.size(), 1);
    QCOMPARE(miModel.property("mediaSourceName").toString(), QStringLiteral("MediaSource"));
    QCOMPARE(miModel.property("mediaSourceIconUrl").toString(), QStringLiteral("http:/localhost/1234.png"));
}

void MediaItemModelShould::be_busy_until_the_items_of_the_opened_container_arrive()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    mediaSrc->setHoldNavigations(true);
    auto busyChangedSpy = QSignalSpy{&miModel, &MediaItemModel::busyChanged};
    QCOMPARE(miModel.property("busy").toBool(), false);

    miModel.activateMediaItem(2);
    QCOMPARE(miModel.property("busy").toBool(), true);
    QCOMPARE(busyChangedSpy.size(), 1);

    mediaSrc->finishPendingNavigation();
    QCOMPARE(miModel.property("busy").toBool(), false);
    QCOMPARE(busyChangedSpy.size(), 2);
    QCOMPARE(miModel.rowCount({}), 3);
}

void MediaItemModelShould::ignore_activations_while_busy()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    mediaSrc->setHoldNavigations(true);
    auto const navigationCount = mediaSrc->navigationCount();

    miModel.activateMediaItem(2);
    miModel.activateMediaItem(2);

    QCOMPARE(mediaSrc->navigationCount(), navigationCount + 1);
}

void MediaItemModelShould::ignore_navigating_back_while_busy()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    miModel.activateMediaItem(2);
    mediaSrc->setHoldNavigations(true);
    auto const navigationCount = mediaSrc->navigationCount();

    miModel.activateMediaItem(2);
    miModel.navigateBack();

    QCOMPARE(mediaSrc->navigationCount(), navigationCount + 1);
    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("2"));
}

void MediaItemModelShould::be_at_the_root_without_a_container_title_until_a_container_is_opened()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    miModel.setMediaSource(mediaSrc);

    QCOMPARE(miModel.property("atRoot").toBool(), true);
    QCOMPARE(miModel.property("containerTitle").toString(), QString{});
}

void MediaItemModelShould::give_the_title_of_the_current_container()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    mediaSrc->setHoldNavigations(true);
    auto containerChangedSpy = QSignalSpy{&miModel, &MediaItemModel::containerChanged};

    miModel.activateMediaItem(2);
    // The title changes only when the Items of the Container are there.
    QCOMPARE(miModel.property("atRoot").toBool(), true);
    QCOMPARE(containerChangedSpy.size(), 0);

    mediaSrc->finishPendingNavigation();
    QCOMPARE(miModel.property("atRoot").toBool(), false);
    QCOMPARE(miModel.property("containerTitle").toString(), QStringLiteral("Container1"));
    QCOMPARE(containerChangedSpy.size(), 1);

    miModel.activateMediaItem(2);
    mediaSrc->finishPendingNavigation();
    QCOMPARE(miModel.property("containerTitle").toString(), QStringLiteral("Container2"));
}

void MediaItemModelShould::return_to_the_parent_container_title_when_navigating_back()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    miModel.activateMediaItem(2);
    miModel.activateMediaItem(2);

    miModel.navigateBack();
    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("1"));
    QCOMPARE(miModel.property("containerTitle").toString(), QStringLiteral("Container1"));
    QCOMPARE(miModel.property("atRoot").toBool(), false);

    miModel.navigateBack();
    QCOMPARE(mediaSrc->lastNavigatedPath(), QStringLiteral("0"));
    QCOMPARE(miModel.property("containerTitle").toString(), QString{});
    QCOMPARE(miModel.property("atRoot").toBool(), true);
    QCOMPARE(miModel.rowCount({}), 5);
}

void MediaItemModelShould::ignore_navigating_back_at_the_root()
{
    auto miModel = MediaItemModel{};
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(mediaSrc);
    auto const navigationCount = mediaSrc->navigationCount();

    miModel.navigateBack();

    QCOMPARE(mediaSrc->navigationCount(), navigationCount);
    QCOMPARE(miModel.property("busy").toBool(), false);
}

void MediaItemModelShould::start_at_the_root_when_the_media_source_changes()
{
    auto miModel = MediaItemModel{};
    auto previousSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mediaSrc = std::make_shared<Multimedia::TestHelper::TestSource>(QString(""), QString(""));
    auto mTester = QAbstractItemModelTester(&miModel, QAbstractItemModelTester::FailureReportingMode::QtTest);
    miModel.setMediaSource(previousSrc);
    miModel.activateMediaItem(2);
    previousSrc->setHoldNavigations(true);
    miModel.activateMediaItem(2);

    miModel.setMediaSource(mediaSrc);

    QCOMPARE(miModel.property("atRoot").toBool(), true);
    QCOMPARE(miModel.property("containerTitle").toString(), QString{});
    QCOMPARE(miModel.property("busy").toBool(), false);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaItemModelShould)
