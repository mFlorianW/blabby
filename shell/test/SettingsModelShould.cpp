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
 * Gives whether the switch in the row of the picked section of the model is on.
 */
bool isChecked(SettingsModel& model, int row)
{
    auto* const rows = model.rows();
    return rows->data(rows->index(row), static_cast<int>(SettingsRowModel::DisplayRole::Checked)).toBool();
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
            .kind =
                rows->data(index, static_cast<int>(SettingsRowModel::DisplayRole::Kind)).value<SettingsRowKind::Kind>(),
        });
    }
    return result;
}

} // namespace

SettingsModelShould::~SettingsModelShould() = default;

void SettingsModelShould::init()
{
    mSettings = std::make_shared<InMemorySettingsStore>();
}

void SettingsModelShould::give_the_role_names_for_the_ui()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};

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
                 {static_cast<int>(SettingsRowModel::DisplayRole::Kind), QByteArray{"kind"}},
                 {static_cast<int>(SettingsRowModel::DisplayRole::Checked), QByteArray{"checked"}},
             }));
}

void SettingsModelShould::list_the_renderers_section_and_then_about()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    auto tester = QAbstractItemModelTester{&model, QAbstractItemModelTester::FailureReportingMode::QtTest};

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.data(model.index(0), static_cast<int>(SettingsModel::DisplayRole::Title)).toString(),
             QStringLiteral("Renderers"));
    QCOMPARE(model.data(model.index(0), static_cast<int>(SettingsModel::DisplayRole::IconSource)).toUrl(),
             QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg")});
    QCOMPARE(model.data(model.index(1), static_cast<int>(SettingsModel::DisplayRole::Title)).toString(),
             QStringLiteral("About"));
    QCOMPARE(model.data(model.index(1), static_cast<int>(SettingsModel::DisplayRole::IconSource)).toUrl(),
             QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/info.svg")});
}

void SettingsModelShould::pick_the_renderers_section_from_the_start()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};

    QCOMPARE(model.currentSection(), 0);
    QCOMPARE(pickedRows(model),
             (QList<SettingsRow>{
                 SettingsRow{.title = QStringLiteral("Remember last Renderer"),
                             .subtitle = QStringLiteral("Make the Renderer you last picked active again when it is "
                                                        "Online"),
                             .value = {},
                             .kind = SettingsRowKind::Switch},
             }));
}

void SettingsModelShould::show_the_remember_last_renderer_switch_on_by_default()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};

    QCOMPARE(isChecked(model, 0), true);
}

void SettingsModelShould::show_the_stored_value_of_the_remember_last_renderer_switch()
{
    mSettings->setValue(SettingsKey::rememberLastRenderer, false);

    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};

    QCOMPARE(isChecked(model, 0), false);
}

void SettingsModelShould::store_the_toggled_value_of_the_remember_last_renderer_switch()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    auto tester = QAbstractItemModelTester{model.rows(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    auto dataChangedSpy = QSignalSpy{model.rows(), &QAbstractItemModel::dataChanged};

    model.setChecked(0, false);

    QCOMPARE(mSettings->value(SettingsKey::rememberLastRenderer), QVariant{false});
    QCOMPARE(isChecked(model, 0), false);
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.at(0).at(0).value<QModelIndex>(), model.rows()->index(0));
    QCOMPARE(dataChangedSpy.at(0).at(2).value<QList<int>>(),
             QList<int>{static_cast<int>(SettingsRowModel::DisplayRole::Checked)});

    model.setChecked(0, true);

    QCOMPARE(mSettings->value(SettingsKey::rememberLastRenderer), QVariant{true});
    QCOMPARE(isChecked(model, 0), true);
    QCOMPARE(dataChangedSpy.size(), 2);
}

void SettingsModelShould::ignore_toggling_a_row_that_is_no_switch()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    model.pickSection(1);
    auto dataChangedSpy = QSignalSpy{model.rows(), &QAbstractItemModel::dataChanged};

    model.setChecked(0, false);
    model.setChecked(-1, false);
    model.setChecked(model.rows()->rowCount(), false);

    QCOMPARE(dataChangedSpy.size(), 0);
    QCOMPARE(mSettings->value(SettingsKey::rememberLastRenderer), QVariant{});
}

void SettingsModelShould::show_the_blabby_version_and_the_qt_version_in_about()
{
    auto model = SettingsModel{Multimedia::Providers{}, blabbyVersion(), mSettings};
    model.pickSection(1);
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
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    model.pickSection(1);

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
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    auto currentSectionSpy = QSignalSpy{&model, &SettingsModel::currentSectionChanged};
    auto resetSpy = QSignalSpy{model.rows(), &QAbstractItemModel::modelReset};

    model.pickSection(-1);
    model.pickSection(2);

    QCOMPARE(model.currentSection(), 0);
    QCOMPARE(currentSectionSpy.size(), 0);
    QCOMPARE(resetSpy.size(), 0);
}

void SettingsModelShould::ignore_picking_the_picked_section_again()
{
    auto model = SettingsModel{twoProviders(), blabbyVersion(), mSettings};
    auto currentSectionSpy = QSignalSpy{&model, &SettingsModel::currentSectionChanged};
    auto resetSpy = QSignalSpy{model.rows(), &QAbstractItemModel::modelReset};

    model.pickSection(0);

    QCOMPARE(currentSectionSpy.size(), 0);
    QCOMPARE(resetSpy.size(), 0);
}

} // namespace Shell

QTEST_MAIN(Shell::SettingsModelShould)
