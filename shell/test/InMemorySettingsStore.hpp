// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "SettingsStore.hpp"
#include <QVariantMap>

namespace Shell
{

/**
 * A @ref Shell::SettingsStore that keeps the settings in memory.
 */
class InMemorySettingsStore : public SettingsStore
{
public:
    /**
     * Default constructor
     */
    InMemorySettingsStore();

    /**
     * Default destructor
     */
    ~InMemorySettingsStore() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(InMemorySettingsStore)

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

private:
    QVariantMap mSettings;
};

} // namespace Shell
