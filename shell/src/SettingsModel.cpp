// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "SettingsModel.hpp"

namespace Shell
{

SettingsModel::SettingsModel(Multimedia::Providers providers,
                             QString blabbyVersion,
                             std::shared_ptr<SettingsStore> settings)
    : mRows{std::move(settings)}
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

    auto about =
        Section{.title = tr("About"),
                .iconSource = QUrl{QStringLiteral("qrc:/qt/qml/Blabby/Shell/icons/material/info.svg")},
                .rows = {
                    SettingsRow{.title = tr("Blabby version"), .subtitle = {}, .value = std::move(blabbyVersion)},
                    SettingsRow{.title = tr("Qt version"), .subtitle = {}, .value = QString::fromLatin1(qVersion())},
                }};
    for (auto const& provider : std::as_const(providers)) {
        about.rows.append(SettingsRow{.title = provider->name(),
                                      .subtitle = tr("Provider"),
                                      .value = provider->version().toString()});
    }
    mSections.append(std::move(about));

    mRows.setRows(mSections.at(mCurrentSection).rows);
}

SettingsModel::~SettingsModel() = default;

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

} // namespace Shell
