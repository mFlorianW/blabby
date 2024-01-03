// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaSourceModelShould.hpp"
#include "MediaSourceModel.hpp"
#include "TestProvider.hpp"
#include "TestProviderLoader.hpp"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>

namespace Shell
{

MediaSourceModelShould::~MediaSourceModelShould() = default;

void MediaSourceModelShould::load_the_provider_and_report_amount_of_media_sources()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    const auto count = model.rowCount();

    QVERIFY2(count == 1,
             QString("The returned ProviderCount \"%1\" is not the expected ProviderCount 1")
                 .arg(QString::number(count))
                 .toLocal8Bit());
}

void MediaSourceModelShould::give_the_correct_role_names_for_the_ui()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expRoleNames = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceName), QByteArray{"mediaSourceName"}),
        std::make_pair(static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceIocnUrl),
                       QByteArray{"mediaSourceIconUrl"})};

    const auto roleNames = model.roleNames();

    QCOMPARE(roleNames, expRoleNames);
}

void MediaSourceModelShould::give_the_name_of_provider_for_valid_index()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expName = QStringLiteral("TestMediaSource");

    const auto providerName =
        model.data(model.index(0), static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceName)).toString();

    QVERIFY2(providerName == expName,
             QString{"The returned ProviderName \"%1\" is not the expected providerName %2"}
                 .arg(providerName, expName)
                 .toLocal8Bit());
}

void MediaSourceModelShould::give_the_iconurl_of_provider_for_valid_index()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    auto expIconUrl = QStringLiteral("http::/127.0.0.1/TestMediaSource.png");

    const auto iconUrl =
        model.data(model.index(0), static_cast<int>(MediaSourceModel::DisplayRole::MediaSourceIocnUrl)).toString();

    QVERIFY2(iconUrl == expIconUrl,
             QString{"The returned ProviderIconUrl \"%1\" is not the expected ProviderIconUrl %2"}
                 .arg(iconUrl, expIconUrl)
                 .toLocal8Bit());
}

void MediaSourceModelShould::handle_new_connected_sources()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto loaderRaw = loader.get();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    qobject_cast<Multimedia::TestHelper::TestProvider *>(loaderRaw->providers().at(0).get())->createNewSource();
    const auto rowCount = model.rowCount();

    QVERIFY2(rowCount == 2,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());
}

void MediaSourceModelShould::handle_disconnected_sources()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>(2);
    auto loaderRaw = loader.get();
    auto model = MediaSourceModel{std::move(loader)};
    auto mTester = QAbstractItemModelTester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    auto rowCount = model.rowCount();
    QVERIFY2(rowCount == 2,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());

    qobject_cast<Multimedia::TestHelper::TestProvider *>(loaderRaw->providers().at(1).get())->removeLastSource();
    rowCount = model.rowCount();

    QVERIFY2(rowCount == 1,
             QString{"The returned rowCount \"%1\" is not the expected rowCount 2"}
                 .arg(QString::number(rowCount))
                 .toLocal8Bit());
}

void MediaSourceModelShould::set_the_active_media_source_property_correctly()
{
    auto loader = std::make_unique<Multimedia::TestHelper::TestProviderLoader>();
    auto loaderRaw = loader.get();
    auto model = MediaSourceModel{std::move(loader)};
    auto mediaSourceActivatedSpy = QSignalSpy{&model, &MediaSourceModel::activeMediaSourceChanged};
    auto expectedSrc = loaderRaw->providers().at(0)->sources().at(0);

    model.activateMediaSource(0);

    QCOMPARE(mediaSourceActivatedSpy.size(), 1);
    QCOMPARE(model.activeMediaSource(), expectedSrc);
    QCOMPARE(model.property("activeMediaSource").value<std::shared_ptr<Multimedia::Source>>(), expectedSrc);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaSourceModelShould);
