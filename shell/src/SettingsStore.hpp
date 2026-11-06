// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QString>
#include <QVariant>

namespace Shell
{

/**
 * The keys of the shell's own settings in the @ref Shell::SettingsStore.
 */
namespace SettingsKey
{
/**
 * Whether the Renderer the user last picked becomes the Active Renderer again by itself, a bool.
 */
constexpr auto rememberLastRenderer = QLatin1StringView{"rememberLastRenderer"};

/**
 * The value of @ref SettingsKey::rememberLastRenderer when it isn't stored.
 */
constexpr auto rememberLastRendererDefault = true;

/**
 * The identity of the Renderer the user last picked, empty when there is none.
 */
constexpr auto lastRenderer = QLatin1StringView{"lastRenderer"};
} // namespace SettingsKey

/**
 * Keeps the shell's settings and the values of the Providers' settings across restarts of Blabby.
 * The values of each Provider are kept apart from the shell's settings and from the other Providers' by its name().
 */
class SettingsStore
{
public:
    /**
     * Default constructor
     */
    SettingsStore() = default;

    /**
     * Default destructor
     */
    virtual ~SettingsStore();

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(SettingsStore)

    /**
     * Gives the stored value of the setting.
     * @param key The key of the setting.
     * @param defaultValue The value that is given when the setting isn't stored.
     * @return The stored value or the default value.
     */
    virtual QVariant value(QString const& key, QVariant const& defaultValue = {}) const = 0;

    /**
     * Stores the value of the setting, it replaces the stored value.
     * @param key The key of the setting.
     * @param value The value to store.
     */
    virtual void setValue(QString const& key, QVariant const& value) = 0;

    /**
     * Removes the stored value of the setting, afterwards its default value is given.
     * @param key The key of the setting.
     */
    virtual void remove(QString const& key) = 0;

    /**
     * Gives the stored value of a setting of the Provider.
     * @param provider The name() of the Provider.
     * @param key The key of the setting.
     * @param defaultValue The value that is given when the setting isn't stored.
     * @return The stored value or the default value.
     */
    virtual QVariant providerValue(QString const& provider,
                                   QString const& key,
                                   QVariant const& defaultValue = {}) const = 0;

    /**
     * Stores the value of a setting of the Provider, it replaces the stored value.
     * @param provider The name() of the Provider.
     * @param key The key of the setting.
     * @param value The value to store.
     */
    virtual void setProviderValue(QString const& provider, QString const& key, QVariant const& value) = 0;
};

} // namespace Shell
