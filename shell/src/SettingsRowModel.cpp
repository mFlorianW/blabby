// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "SettingsRowModel.hpp"

namespace Shell
{

SettingsRowModel::SettingsRowModel(std::shared_ptr<SettingsStore> settings)
    : mSettings{std::move(settings)}
{
}

SettingsRowModel::~SettingsRowModel() = default;

int SettingsRowModel::rowCount(QModelIndex const& parent) const noexcept
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(mRows.size());
}

QHash<int, QByteArray> SettingsRowModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        {static_cast<int>(DisplayRole::Title), QByteArray{"title"}},
        {static_cast<int>(DisplayRole::Subtitle), QByteArray{"subtitle"}},
        {static_cast<int>(DisplayRole::Value), QByteArray{"value"}},
        {static_cast<int>(DisplayRole::Kind), QByteArray{"kind"}},
        {static_cast<int>(DisplayRole::Checked), QByteArray{"checked"}},
    };
    return roles;
}

QVariant SettingsRowModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or index.row() >= mRows.size()) {
        return {};
    }

    auto const& row = mRows.at(index.row());
    switch (static_cast<DisplayRole>(role)) {
    case DisplayRole::Title:
        return row.title;
    case DisplayRole::Subtitle:
        return row.subtitle;
    case DisplayRole::Value:
        return row.value;
    case DisplayRole::Kind:
        return QVariant::fromValue(row.kind);
    case DisplayRole::Checked:
        return row.kind == SettingsRowKind::Switch and mSettings->value(row.key, row.defaultChecked).toBool();
    }
    return {};
}

void SettingsRowModel::setRows(QList<SettingsRow> rows)
{
    beginResetModel();
    mRows = std::move(rows);
    endResetModel();
}

void SettingsRowModel::setChecked(int row, bool checked)
{
    if (row < 0 or row >= mRows.size() or mRows.at(row).kind != SettingsRowKind::Switch) {
        return;
    }

    mSettings->setValue(mRows.at(row).key, checked);
    auto const changedIndex = index(row);
    Q_EMIT dataChanged(changedIndex, changedIndex, {static_cast<int>(DisplayRole::Checked)});
}

} // namespace Shell
