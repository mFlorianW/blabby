// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ProviderLoader.hpp"
#include "SettingsRowModel.hpp"
#include <QAbstractListModel>
#include <QUrl>

namespace Shell
{

/**
 * The sections of the Settings screen, one model row per section, and the rows of the picked section.
 * The first section is picked from the start. The last section is About, it shows the Blabby version, the Qt version
 * and every loaded Provider with its version.
 */
class SettingsModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * The index of the picked section.
     */
    Q_PROPERTY(int currentSection READ currentSection NOTIFY currentSectionChanged)

    /**
     * The rows of the picked section.
     */
    Q_PROPERTY(Shell::SettingsRowModel* rows READ rows CONSTANT)

public:
    /**
     * The roles for the UI to query the data of a section.
     */
    enum class DisplayRole
    {
        Title = Qt::UserRole + 1,
        IconSource,
    };
    Q_ENUM(DisplayRole)

    /**
     * Creates the sections for the passed Providers.
     * @param providers The loaded Providers, they are already initialized.
     * @param blabbyVersion The version of Blabby shown in About.
     */
    SettingsModel(Multimedia::Providers providers, QString blabbyVersion);

    /**
     * Default destructor
     */
    ~SettingsModel() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(SettingsModel)

    /**
     * Gives the number of sections.
     */
    int rowCount(QModelIndex const& parent = QModelIndex()) const noexcept override;

    /**
     * Gives the role names for the UI to access the data of a section.
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * Gives the data of the section at index for the role.
     * @return The data or an empty QVariant for an invalid index or role.
     */
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const noexcept override;

    /**
     * Gives the index of the picked section.
     */
    int currentSection() const noexcept;

    /**
     * Gives the rows of the picked section.
     * @note The caller doesn't take the ownership of the returned pointer.
     */
    SettingsRowModel* rows() noexcept;

    /**
     * Picks the section at index and shows its rows. An invalid index or the picked section changes nothing.
     * @param index The index of the section to pick.
     */
    Q_INVOKABLE void pickSection(int index);

Q_SIGNALS:
    /**
     * This signal is emitted when another section is picked.
     */
    void currentSectionChanged();

private:
    struct Section
    {
        QString title;
        QUrl iconSource;
        QList<SettingsRow> rows;
    };

    QList<Section> mSections;
    int mCurrentSection = 0;
    SettingsRowModel mRows;
};

} // namespace Shell
