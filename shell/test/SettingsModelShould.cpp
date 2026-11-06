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
 * Gives a fake Provider "Fake" titled "Fake settings" with two switches, "shuffle" off and "autoScan" on by default,
 * and the action "rescan".
 */
std::shared_ptr<Multimedia::TestHelper::TestProvider> providerWithSettings(
    QString name = QStringLiteral("Fake"),
    QString title = QStringLiteral("Fake settings"))
{
    auto provider = std::make_shared<Multimedia::TestHelper::TestProvider>(std::move(name));
    provider->setDisplayTitle(std::move(title));
    provider->setSettings({
        Multimedia::ProviderSetting{.key = QStringLiteral("shuffle"),
                                    .title = QStringLiteral("Shuffle"),
                                    .subtitle = QStringLiteral("Play in random order"),
                                    .kind = Multimedia::ProviderSetting::Kind::Switch,
                                    .defaultValue = false},
        Multimedia::ProviderSetting{.key = QStringLiteral("autoScan"),
                                    .title = QStringLiteral("Scan automatically"),
                                    .subtitle = {},
                                    .kind = Multimedia::ProviderSetting::Kind::Switch,
                                    .defaultValue = true},
        Multimedia::ProviderSetting{.key = QStringLiteral("rescan"),
                                    .title = QStringLiteral("Rescan network"),
                                    .subtitle = QStringLiteral("Search for media servers again"),
                                    .kind = Multimedia::ProviderSetting::Kind::Action},
    });
    return provider;
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

void SettingsModelShould::show_the_display_title_of_a_provider_in_about()
{
    auto model = SettingsModel{Multimedia::Providers{providerWithSettings()}, blabbyVersion(), mSettings};
    model.pickSection(2);

    QCOMPARE(pickedRows(model).at(2),
             (SettingsRow{.title = QStringLiteral("Fake settings"),
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

void SettingsModelShould::pass_each_provider_its_stored_values_or_the_defaults_at_start()
{
    auto provider = providerWithSettings();
    mSettings->setProviderValue(QStringLiteral("Fake"), QStringLiteral("shuffle"), true);
    // Stored for a key the Provider no longer describes.
    mSettings->setProviderValue(QStringLiteral("Fake"), QStringLiteral("gone"), true);
    // Stored for another Provider.
    mSettings->setProviderValue(QStringLiteral("Other"), QStringLiteral("autoScan"), false);

    auto model = SettingsModel{Multimedia::Providers{provider}, blabbyVersion(), mSettings};

    QCOMPARE(provider->passedValues(),
             (QList<std::pair<QString, QVariant>>{
                 {QStringLiteral("shuffle"), QVariant{true}},
                 {QStringLiteral("autoScan"), QVariant{true}},
             }));
    QCOMPARE(provider->triggeredActions(), QStringList{});
}

void SettingsModelShould::list_a_section_for_each_provider_with_settings_ordered_by_title_between_renderers_and_about()
{
    auto const providers = Multimedia::Providers{
        providerWithSettings(QStringLiteral("Zeta"), QStringLiteral("Zeta servers")),
        std::make_shared<Multimedia::TestHelper::TestProvider>(QStringLiteral("Plain")),
        providerWithSettings(QStringLiteral("Alpha"), QStringLiteral("Alpha sticks")),
    };
    auto model = SettingsModel{providers, blabbyVersion(), mSettings};
    auto tester = QAbstractItemModelTester{&model, QAbstractItemModelTester::FailureReportingMode::QtTest};

    auto titles = QStringList{};
    for (auto row = 0; row < model.rowCount(); ++row) {
        titles.append(model.data(model.index(row), static_cast<int>(SettingsModel::DisplayRole::Title)).toString());
    }
    QCOMPARE(titles,
             (QStringList{QStringLiteral("Renderers"),
                          QStringLiteral("Alpha sticks"),
                          QStringLiteral("Zeta servers"),
                          QStringLiteral("About")}));
    QCOMPARE(model.data(model.index(1), static_cast<int>(SettingsModel::DisplayRole::IconSource)).toUrl(),
             QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg")});
}

void SettingsModelShould::show_the_settings_of_a_provider_in_its_section()
{
    mSettings->setProviderValue(QStringLiteral("Fake"), QStringLiteral("shuffle"), true);
    auto model = SettingsModel{Multimedia::Providers{providerWithSettings()}, blabbyVersion(), mSettings};
    model.pickSection(1);
    auto tester = QAbstractItemModelTester{model.rows(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    QCOMPARE(pickedRows(model),
             (QList<SettingsRow>{
                 SettingsRow{.title = QStringLiteral("Shuffle"),
                             .subtitle = QStringLiteral("Play in random order"),
                             .value = {},
                             .kind = SettingsRowKind::Switch},
                 SettingsRow{.title = QStringLiteral("Scan automatically"),
                             .subtitle = {},
                             .value = {},
                             .kind = SettingsRowKind::Switch},
                 SettingsRow{.title = QStringLiteral("Rescan network"),
                             .subtitle = QStringLiteral("Search for media servers again"),
                             .value = {},
                             .kind = SettingsRowKind::Action},
             }));
    QCOMPARE(isChecked(model, 0), true);
    QCOMPARE(isChecked(model, 1), true);
    QCOMPARE(isChecked(model, 2), false);
}

void SettingsModelShould::store_a_toggled_provider_switch_and_pass_it_to_the_provider()
{
    auto provider = providerWithSettings();
    auto model = SettingsModel{Multimedia::Providers{provider}, blabbyVersion(), mSettings};
    model.pickSection(1);
    auto dataChangedSpy = QSignalSpy{model.rows(), &QAbstractItemModel::dataChanged};
    auto const valuesAtStart = provider->passedValues().size();

    model.setChecked(0, true);

    QCOMPARE(mSettings->providerValue(QStringLiteral("Fake"), QStringLiteral("shuffle")), QVariant{true});
    QCOMPARE(mSettings->value(QStringLiteral("shuffle")), QVariant{});
    QCOMPARE(isChecked(model, 0), true);
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(provider->passedValues().mid(valuesAtStart),
             (QList<std::pair<QString, QVariant>>{{QStringLiteral("shuffle"), QVariant{true}}}));
}

void SettingsModelShould::pass_a_tapped_action_to_the_provider()
{
    auto provider = providerWithSettings();
    auto model = SettingsModel{Multimedia::Providers{provider}, blabbyVersion(), mSettings};
    model.pickSection(1);

    model.triggerAction(2);
    model.triggerAction(2);

    QCOMPARE(provider->triggeredActions(), (QStringList{QStringLiteral("rescan"), QStringLiteral("rescan")}));
}

void SettingsModelShould::ignore_triggering_a_row_that_is_no_action()
{
    auto provider = providerWithSettings();
    auto model = SettingsModel{Multimedia::Providers{provider}, blabbyVersion(), mSettings};
    model.pickSection(1);
    auto const valuesAtStart = provider->passedValues().size();

    model.triggerAction(0);
    model.triggerAction(-1);
    model.triggerAction(model.rows()->rowCount());

    QCOMPARE(provider->triggeredActions(), QStringList{});
    QCOMPARE(provider->passedValues().size(), valuesAtStart);
}

} // namespace Shell

QTEST_MAIN(Shell::SettingsModelShould)
