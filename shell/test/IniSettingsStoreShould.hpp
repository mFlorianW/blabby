// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QObject>
#include <QTemporaryDir>

namespace Shell
{

class IniSettingsStoreShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~IniSettingsStoreShould() override;
    Q_DISABLE_COPY_MOVE(IniSettingsStoreShould)

private:
    QString storePath() const;

    std::unique_ptr<QTemporaryDir> mDir;

private Q_SLOTS:
    void init();
    void give_the_stored_value();
    void replace_the_stored_value();
    void give_the_default_for_a_missing_key();
    void give_the_default_for_a_removed_key();
    void give_the_stored_values_after_a_restart();
    void forget_a_removed_key_across_a_restart();
    void give_the_defaults_when_the_file_is_missing();
    void give_the_defaults_when_the_file_is_corrupt();
    void create_the_missing_directory_on_a_change();
    void keep_the_values_of_providers_apart_by_name();
    void give_the_stored_provider_values_after_a_restart();
};

} // namespace Shell
