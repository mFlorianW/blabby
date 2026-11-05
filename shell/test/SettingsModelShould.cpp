// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "SettingsModelShould.hpp"
#include "SettingsModel.hpp"
#include "TestProvider.hpp"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>

namespace Shell
{

namespace
{

/**
 * Gives the Blabby version the model is created with.
 */
QString blabbyVersion()
{
    return QStringLiteral("1.2.3");
}

/**
 * Gives two fake Providers, "Media servers" and "USB sticks", both in version 0.1.0.
 */
Multimedia::Providers twoProviders()
{
    return Multimedia::Providers{
        std::make_shared<Multimedia::TestHelper::TestProvider>(QStringLiteral("Media servers")),
        std::make_shared<Multimedia::TestHelper::TestProvider>(QStringLiteral("USB sticks")),
    };
}

/**
 * Gives the rows of the picked section of the model.
 */
QList<SettingsRow> pickedRows(SettingsModel& model)
{
    auto* const rows = model.rows();
    auto result = QList<SettingsRow>{};
    for (auto row = 0; row < rows->rowCount(); ++row) {
        auto const index = rows->index(row);
        result.append(SettingsRow{
            .title = rows->data(index, static_cast<int>(SettingsRowModel::DisplayRole::Title)).toString(),
            .subtitle = rows->data(index, static_cast<int>(SettingsRowModel::DisplayRole::Subtitle)).toString(),
            .value = rows->data(index, static_cast<int>(SettingsRowModel::DisplayRole::Value)).toString(),
        });
    }
    return result;
}

} // namespace

SettingsModelShould::~SettingsModelShould() = default;

void SettingsModelShould::give_the_role_names_for_the_ui()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};

    QCOMPARE(model.roleNames(),
             (QHash<int, QByteArray>{
                 {static_cast<int>(SettingsModel::DisplayRole::Title), QByteArray{"title"}},
                 {static_cast<int>(SettingsModel::DisplayRole::IconSource), QByteArray{"iconSource"}},
             }));
    QCOMPARE(model.rows()->roleNames(),
             (QHash<int, QByteArray>{
                 {static_cast<int>(SettingsRowModel::DisplayRole::Title), QByteArray{"title"}},
                 {static_cast<int>(SettingsRowModel::DisplayRole::Subtitle), QByteArray{"subtitle"}},
                 {static_cast<int>(SettingsRowModel::DisplayRole::Value), QByteArray{"value"}},
             }));
}

void SettingsModelShould::list_the_about_section()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};
    auto tester = QAbstractItemModelTester{&model, QAbstractItemModelTester::FailureReportingMode::QtTest};

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0), static_cast<int>(SettingsModel::DisplayRole::Title)).toString(),
             QStringLiteral("About"));
    QCOMPARE(model.data(model.index(0), static_cast<int>(SettingsModel::DisplayRole::IconSource)).toUrl(),
             QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/info.svg")});
}

void SettingsModelShould::pick_the_first_section_from_the_start()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};

    QCOMPARE(model.currentSection(), 0);
    QCOMPARE(model.rows()->rowCount(), 4);
}

void SettingsModelShould::show_the_blabby_version_and_the_qt_version_in_about()
{
    auto model = SettingsModel{Multimedia::Providers{}, blabbyVersion()};
    auto tester = QAbstractItemModelTester{model.rows(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    QCOMPARE(pickedRows(model),
             (QList<SettingsRow>{
                 SettingsRow{.title = QStringLiteral("Blabby version"), .subtitle = {}, .value = blabbyVersion()},
                 SettingsRow{.title = QStringLiteral("Qt version"),
                             .subtitle = {},
                             .value = QString::fromLatin1(qVersion())},
             }));
}

void SettingsModelShould::show_each_loaded_provider_with_its_version_in_about()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};

    auto const rows = pickedRows(model);

    QCOMPARE(rows.size(), 4);
    QCOMPARE(rows.at(2),
             (SettingsRow{.title = QStringLiteral("Media servers"),
                          .subtitle = QStringLiteral("Provider"),
                          .value = QStringLiteral("0.1.0")}));
    QCOMPARE(rows.at(3),
             (SettingsRow{.title = QStringLiteral("USB sticks"),
                          .subtitle = QStringLiteral("Provider"),
                          .value = QStringLiteral("0.1.0")}));
}

void SettingsModelShould::ignore_picking_an_invalid_section()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};
    auto currentSectionSpy = QSignalSpy{&model, &SettingsModel::currentSectionChanged};
    auto resetSpy = QSignalSpy{model.rows(), &QAbstractItemModel::modelReset};

    model.pickSection(-1);
    model.pickSection(1);

    QCOMPARE(model.currentSection(), 0);
    QCOMPARE(currentSectionSpy.size(), 0);
    QCOMPARE(resetSpy.size(), 0);
}

void SettingsModelShould::ignore_picking_the_picked_section_again()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion()};
    auto currentSectionSpy = QSignalSpy{&model, &SettingsModel::currentSectionChanged};
    auto resetSpy = QSignalSpy{model.rows(), &QAbstractItemModel::modelReset};

    model.pickSection(0);

    QCOMPARE(currentSectionSpy.size(), 0);
    QCOMPARE(resetSpy.size(), 0);
}

} // namespace Shell

QTEST_MAIN(Shell::SettingsModelShould)
