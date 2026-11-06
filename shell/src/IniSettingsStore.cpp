// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "IniSettingsStore.hpp"
#include "LoggingCategories.hpp"

namespace Shell
{

namespace
{
/**
 * The shell's own settings are kept in their own group, apart from the ones of the Providers.
 */
QString shellKey(QString const& key)
{
    return QStringLiteral("shell/") + key;
}

/**
 * The settings of each Provider are kept in a group of their own, named after the Provider.
 */
QString providerKey(QString const& provider, QString const& key)
{
    return QStringLiteral("providers/") + provider + QLatin1Char{'/'} + key;
}
} // namespace

IniSettingsStore::IniSettingsStore(QString const& filePath)
    : mSettings{filePath, QSettings::IniFormat}
{
    if (mSettings.status() != QSettings::NoError) {
        qCWarning(shell) << "Failed to read the settings from" << filePath << "Error:" << mSettings.status();
    }
}

IniSettingsStore::~IniSettingsStore() = default;

QVariant IniSettingsStore::value(QString const& key, QVariant const& defaultValue) const
{
    return mSettings.value(shellKey(key), defaultValue);
}

void IniSettingsStore::setValue(QString const& key, QVariant const& value)
{
    mSettings.setValue(shellKey(key), value);
    sync();
}

void IniSettingsStore::remove(QString const& key)
{
    mSettings.remove(shellKey(key));
    sync();
}

QVariant IniSettingsStore::providerValue(QString const& provider,
                                         QString const& key,
                                         QVariant const& defaultValue) const
{
    return mSettings.value(providerKey(provider, key), defaultValue);
}

void IniSettingsStore::setProviderValue(QString const& provider, QString const& key, QVariant const& value)
{
    mSettings.setValue(providerKey(provider, key), value);
    sync();
}

void IniSettingsStore::sync()
{
    // QSettings writes changes deferred, they are written right away so that none is lost on a crash.
    mSettings.sync();
    if (mSettings.status() != QSettings::NoError) {
        qCWarning(shell) << "Failed to write the settings to" << mSettings.fileName() << "Error:" << mSettings.status();
    }
}

} // namespace Shell
