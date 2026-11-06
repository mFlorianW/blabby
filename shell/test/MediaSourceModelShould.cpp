// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaSourceModelShould.hpp"
#include "LoadProviders.hpp"
#include "MediaSourceModel.hpp"
#include "TestProvider.hpp"
#include "TestProviderLoader.hpp"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>
#include <algorithm>

namespace Shell
{

namespace
{
constexpr auto activeRole = static_cast<int>(MediaSourceModel::DisplayRole::Active);

bool isActive(MediaSourceModel const& model, int row)
{
    return model.data(model.index(row), activeRole).toBool();
}
} // namespace

MediaSourceModelShould::~MediaSourceModelShould() = default;

void MediaSourceModelShould::load_the_provider_and_report_amount_of_media_sources()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    auto const count = model.rowCount();

    QVERIFY2(count == 1,
             QString("The returned ProviderCount \"%1\" is not the expected ProviderCount 1")
                 .arg(QString::number(count))
                 .toLocal8Bit());
}

void MediaSourceModelShould::give_the_correct_role_names_for_the_ui()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expRoleNames = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceName), QByteArray{"mediaSourceName"}),
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceIocnUrl),
                       QByteArray{"mediaSourceIconUrl"}),
        std::make_pair(activeRole, QByteArray{"active"})};

    auto const roleNames = model.roleNames();

    QCOMPARE(roleNames, expRoleNames);
}

void MediaSourceModelShould::give_the_name_of_provider_for_valid_index()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expName = QStringLiteral("TestMediaSource");

    auto const providerName =
        model.data(model.index(0), static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceName)).toString();

    QVERIFY2(providerName == expName,
             QString{"The returned ProviderName \"%1\" is not the expected providerName %2"}
                 .arg(providerName, expName)
                 .toLocal8Bit());
}

void MediaSourceModelShould::give_the_iconurl_of_provider_for_valid_index()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expIconUrl = QStringLiteral("http::/127.0.0.1/TestMediaSource.png");

    auto const iconUrl =
        model.data(model.index(0), static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceIocnUrl)).toString();

    QVERIFY2(iconUrl == expIconUrl,
             QString{"The returned ProviderIconUrl \"%1\" is not the expected ProviderIconUrl %2"}
                 .arg(iconUrl, expIconUrl)
                 .toLocal8Bit());
}

void MediaSourceModelShould::handle_new_connected_sources()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(0).get())->createNewSource();
    auto const rowCount = model.rowCount();

    QVERIFY2(rowCount == 2,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());
}

void MediaSourceModelShould::handle_disconnected_sources()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    auto rowCount = model.rowCount();
    QVERIFY2(rowCount == 2,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());

    qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(1).get())->removeLastSource();
    rowCount = model.rowCount();

    QVERIFY2(rowCount == 1,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());
}

void MediaSourceModelShould::set_the_active_media_source_property_correctly()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mediaSourceActivatedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};
    auto expectedSrc = loader.providers().at(0)->sources().at(0);

    model.activateMediaSource(0);

    QCOMPARE(mediaSourceActivatedSpy.size(), 1);
    QCOMPARE(model.activeMediaSource(), expectedSrc);
    QCOMPARE(model.property("activeMediaSource").value<std::shared_ptr<Multimedia::Source>>(), expectedSrc);
}

void MediaSourceModelShould::ignore_activating_the_active_media_source_again()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    model.activateMediaSource(0);
    auto mediaSourceActivatedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};

    model.activateMediaSource(0);

    QCOMPARE(mediaSourceActivatedSpy.size(), 0);
    QCOMPARE(model.activeMediaSource(), loader.providers().at(0)->sources().at(0));
}

void MediaSourceModelShould::ignore_activating_an_invalid_index()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mediaSourceActivatedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};

    model.activateMediaSource(-1);
    model.activateMediaSource(1);

    QCOMPARE(mediaSourceActivatedSpy.size(), 0);
    QCOMPARE(model.activeMediaSource(), nullptr);
}

void MediaSourceModelShould::have_no_active_media_source_when_the_active_media_source_is_removed()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    model.activateMediaSource(1);
    auto activeChangedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};
    auto activeRemovedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceDisappeared};

    qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(1).get())->removeLastSource();

    QCOMPARE(activeChangedSpy.size(), 1);
    QCOMPARE(model.activeMediaSource(), nullptr);
    QCOMPARE(activeRemovedSpy.size(), 1);
    QCOMPARE(activeRemovedSpy.at(0).at(0).toString(), QStringLiteral("TestMediaSource"));
}

void MediaSourceModelShould::not_activate_a_reappearing_media_source()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto* provider = qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(0).get());
    model.activateMediaSource(0);
    provider->removeLastSource();
    auto activeChangedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};

    provider->createNewSource();

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(activeChangedSpy.size(), 0);
    QCOMPARE(model.activeMediaSource(), nullptr);
}

void MediaSourceModelShould::keep_the_active_media_source_when_another_media_source_is_removed()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    model.activateMediaSource(0);
    auto activeChangedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};
    auto activeRemovedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceDisappeared};

    qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(1).get())->removeLastSource();

    QCOMPARE(activeChangedSpy.size(), 0);
    QCOMPARE(activeRemovedSpy.size(), 0);
    QCOMPARE(model.activeMediaSource(), loader.providers().at(0)->sources().at(0));
}

void MediaSourceModelShould::mark_no_row_as_active_without_an_active_media_source()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    QCOMPARE(isActive(model, 0), false);
    QCOMPARE(isActive(model, 1), false);
}

void MediaSourceModelShould::mark_only_the_row_of_the_active_media_source_as_active()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    model.activateMediaSource(1);

    QCOMPARE(isActive(model, 0), false);
    QCOMPARE(isActive(model, 1), true);
}

void MediaSourceModelShould::notify_the_active_role_of_the_previous_and_the_new_active_row()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    model.activateMediaSource(0);
    auto dataChangedSpy = QSignalSpy{&model, &MediaSourceModel::dataChanged};

    model.activateMediaSource(1);

    QCOMPARE(dataChangedSpy.size(), 2);
    auto changedRows = QList<int>{};
    for (auto const& args : std::as_const(dataChangedSpy)) {
        auto const topLeft = args.at(0).value<QModelIndex>();
        QCOMPARE(args.at(1).value<QModelIndex>(), topLeft);
        QCOMPARE(args.at(2).value<QList<int>>(), QList<int>({activeRole}));
        changedRows.append(topLeft.row());
    }
    std::sort(changedRows.begin(), changedRows.end());
    QCOMPARE(changedRows, QList<int>({0, 1}));
}

void MediaSourceModelShould::keep_the_active_role_on_the_active_media_source_when_rows_are_removed_and_added()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    auto model = MediaSourceModel{loadProviders(loader, {})};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto* firstProvider = qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(0).get());
    model.activateMediaSource(1);

    firstProvider->removeLastSource();

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(isActive(model, 0), true);

    firstProvider->createNewSource();

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(isActive(model, 0), true);
    QCOMPARE(isActive(model, 1), false);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaSourceModelShould);
