// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QAbstractListModel>

namespace Shell
{

/**
 * A row of a section on the Settings screen.
 */
struct SettingsRow
{
    /**
     * The title of the row, e.g. "Qt version".
     */
    QString title;

    /**
     * A short description shown below the title, empty when the row has none.
     */
    QString subtitle;

    /**
     * The value shown at the trailing edge of the row, e.g. "6.8.2".
     */
    QString value;

    friend bool operator==(SettingsRow const&, SettingsRow const&) = default;
};

/**
 * The rows of the section that is picked on the Settings screen, one model row per @ref Shell::SettingsRow.
 * The @ref Shell::SettingsModel replaces the rows when another section is picked.
 */
class SettingsRowModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * The roles for the UI to query the data of a row.
     */
    enum class DisplayRole
    {
        Title = Qt::UserRole + 1,
        Subtitle,
        Value,
    };
    Q_ENUM(DisplayRole)

    /**
     * Creates a model without rows.
     */
    SettingsRowModel();

    /**
     * Default destructor
     */
    ~SettingsRowModel() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(SettingsRowModel)

    /**
     * Gives the number of rows.
     */
    int rowCount(QModelIndex const& parent = QModelIndex()) const noexcept override;

    /**
     * Gives the role names for the UI to access the data of a row.
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * Gives the data of the row at index for the role.
     * @return The data or an empty QVariant for an invalid index or role.
     */
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const noexcept override;

    /**
     * Replaces the rows of the model.
     * @param rows The rows of the picked section.
     */
    void setRows(QList<SettingsRow> rows);

private:
    QList<SettingsRow> mRows;
};

} // namespace Shell
