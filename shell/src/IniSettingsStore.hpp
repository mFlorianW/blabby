// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "SettingsStore.hpp"
#include <QSettings>

namespace Shell
{

/**
 * A @ref Shell::SettingsStore that keeps the settings in an INI file.
 * Every change is written to the file immediately.
 */
class IniSettingsStore : public SettingsStore
{
public:
    /**
     * Creates an instance of the IniSettingsStore with the settings of the file.
     * A missing or corrupt file gives no stored settings.
     * @param filePath The path of the INI file, missing parent directories are created on a change.
     */
    explicit IniSettingsStore(QString const& filePath);

    /**
     * Default destructor
     */
    ~IniSettingsStore() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(IniSettingsStore)

    /**
     * @copydoc Shell::SettingsStore::value
     */
    QVariant value(QString const& key, QVariant const& defaultValue = {}) const override;

    /**
     * @copydoc Shell::SettingsStore::setValue
     */
    void setValue(QString const& key, QVariant const& value) override;

    /**
     * @copydoc Shell::SettingsStore::remove
     */
    void remove(QString const& key) override;

    /**
     * @copydoc Shell::SettingsStore::providerValue
     */
    QVariant providerValue(QString const& provider,
                           QString const& key,
                           QVariant const& defaultValue = {}) const override;

    /**
     * @copydoc Shell::SettingsStore::setProviderValue
     */
    void setProviderValue(QString const& provider, QString const& key, QVariant const& value) override;

private:
    void sync();

    QSettings mSettings;
};

} // namespace Shell
