// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "InMemorySettingsStore.hpp"

namespace Shell
{

InMemorySettingsStore::InMemorySettingsStore() = default;

InMemorySettingsStore::~InMemorySettingsStore() = default;

QVariant InMemorySettingsStore::value(QString const& key, QVariant const& defaultValue) const
{
    return mSettings.value(key, defaultValue);
}

void InMemorySettingsStore::setValue(QString const& key, QVariant const& value)
{
    mSettings.insert(key, value);
}

void InMemorySettingsStore::remove(QString const& key)
{
    mSettings.remove(key);
}

} // namespace Shell
