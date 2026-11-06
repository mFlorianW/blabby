// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "SettingsModel.hpp"
#include <algorithm>

namespace Shell
{

SettingsModel::SettingsModel(Multimedia::Providers providers,
                             QString blabbyVersion,
                             std::shared_ptr<SettingsStore> settings)
    : mRows{settings}
{
    mSections.append(
        Section{.title = tr("Renderers"),
                .iconSource = QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/speaker.svg")},
                .rows = {
                    SettingsRow{.title = tr("Remember last Renderer"),
                                .subtitle = tr("Make the Renderer you last picked active again when it is Online"),
                                .value = {},
                                .kind = SettingsRowKind::Switch,
                                .key = SettingsKey::rememberLastRenderer,
                                .defaultChecked = SettingsKey::rememberLastRendererDefault},
                }});

    auto providerSections = QList<Section>{};
    for (auto const& provider : std::as_const(providers)) {
        auto section = providerSection(provider);
        // The Provider gets the values of its switches at start, the stored ones or else the defaults.
        for (auto const& row : std::as_const(section.rows)) {
            if (row.kind == SettingsRowKind::Switch) {
                provider->setSettingValue(
                    row.key,
                    settings->providerValue(provider->name(), row.key, row.defaultChecked).toBool());
            }
        }
        if (not section.rows.isEmpty()) {
            providerSections.append(std::move(section));
        }
    }
    std::ranges::sort(providerSections, [](Section const& left, Section const& right) {
        return QString::localeAwareCompare(left.title, right.title) < 0;
    });
    mSections.append(providerSections);

    auto about =
        Section{.title = tr("About"),
                .iconSource = QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/info.svg")},
                .rows = {
                    SettingsRow{.title = tr("Blabby version"), .subtitle = {}, .value = std::move(blabbyVersion)},
                    SettingsRow{.title = tr("Qt version"), .subtitle = {}, .value = QString::fromLatin1(qVersion())},
                }};
    for (auto const& provider : std::as_const(providers)) {
        about.rows.append(SettingsRow{.title = providerTitle(*provider),
                                      .subtitle = tr("Provider"),
                                      .value = provider->version().toString()});
    }
    mSections.append(std::move(about));

    mRows.setRows(mSections.at(mCurrentSection).rows);
}

SettingsModel::~SettingsModel() = default;

SettingsModel::Section SettingsModel::providerSection(std::shared_ptr<Multimedia::Provider> const& provider)
{
    auto section = Section{.title = providerTitle(*provider),
                           .iconSource = QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/dns.svg")},
                           .rows = {}};
    auto const settings = provider->settings();
    for (auto const& setting : settings) {
        auto const isSwitch = setting.kind == Multimedia::ProviderSetting::Kind::Switch;
        section.rows.append(SettingsRow{.title = setting.title,
                                        .subtitle = setting.subtitle,
                                        .value = {},
                                        .kind = isSwitch ? SettingsRowKind::Switch : SettingsRowKind::Action,
                                        .key = setting.key,
                                        .defaultChecked = isSwitch and setting.defaultValue,
                                        .provider = provider});
    }
    return section;
}

QString SettingsModel::providerTitle(Multimedia::Provider const& provider)
{
    auto title = provider.displayTitle();
    return title.isEmpty() ? provider.name() : title;
}

int SettingsModel::rowCount(QModelIndex const& parent) const noexcept
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(mSections.size());
}

QHash<int, QByteArray> SettingsModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        {static_cast<int>(DisplayRole::Title), QByteArray{"title"}},
        {static_cast<int>(DisplayRole::IconSource), QByteArray{"iconSource"}},
    };
    return roles;
}

QVariant SettingsModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or index.row() >= mSections.size()) {
        return {};
    }

    auto const& section = mSections.at(index.row());
    switch (static_cast<DisplayRole>(role)) {
    case DisplayRole::Title:
        return section.title;
    case DisplayRole::IconSource:
        return section.iconSource;
    }
    return {};
}

int SettingsModel::currentSection() const noexcept
{
    return mCurrentSection;
}

SettingsRowModel* SettingsModel::rows() noexcept
{
    return &mRows;
}

void SettingsModel::pickSection(int index)
{
    if (index < 0 or index >= mSections.size() or index == mCurrentSection) {
        return;
    }

    mCurrentSection = index;
    mRows.setRows(mSections.at(mCurrentSection).rows);
    Q_EMIT currentSectionChanged();
}

void SettingsModel::setChecked(int row, bool checked)
{
    mRows.setChecked(row, checked);
}

void SettingsModel::triggerAction(int row)
{
    mRows.triggerAction(row);
}

} // namespace Shell
