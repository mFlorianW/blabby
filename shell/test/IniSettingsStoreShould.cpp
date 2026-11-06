// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "IniSettingsStoreShould.hpp"
#include "IniSettingsStore.hpp"
#include <QFile>
#include <QTest>

namespace Shell
{

namespace
{
constexpr auto switchKey = QLatin1StringView{"switch"};
constexpr auto textKey = QLatin1StringView{"text"};
} // namespace

IniSettingsStoreShould::~IniSettingsStoreShould() = default;

void IniSettingsStoreShould::init()
{
    mDir = std::make_unique<QTemporaryDir>();
    QVERIFY(mDir->isValid());
}

QString IniSettingsStoreShould::storePath() const
{
    return mDir->filePath(QStringLiteral("settings.ini"));
}

void IniSettingsStoreShould::give_the_stored_value()
{
    auto store = IniSettingsStore{storePath()};

    store.setValue(switchKey, false);
    store.setValue(textKey, QStringLiteral("uuid:kitchen"));

    QCOMPARE(store.value(switchKey, true).toBool(), false);
    QCOMPARE(store.value(textKey), QVariant{QStringLiteral("uuid:kitchen")});
}

void IniSettingsStoreShould::replace_the_stored_value()
{
    auto store = IniSettingsStore{storePath()};
    store.setValue(textKey, QStringLiteral("uuid:kitchen"));

    store.setValue(textKey, QStringLiteral("uuid:bathroom"));

    QCOMPARE(store.value(textKey), QVariant{QStringLiteral("uuid:bathroom")});
}

void IniSettingsStoreShould::give_the_default_for_a_missing_key()
{
    auto store = IniSettingsStore{storePath()};
    store.setValue(textKey, QStringLiteral("uuid:kitchen"));

    QCOMPARE(store.value(switchKey, true).toBool(), true);
    QCOMPARE(store.value(switchKey), QVariant{});
}

void IniSettingsStoreShould::give_the_default_for_a_removed_key()
{
    auto store = IniSettingsStore{storePath()};
    store.setValue(textKey, QStringLiteral("uuid:kitchen"));

    store.remove(textKey);

    QCOMPARE(store.value(textKey, QString{}), QVariant{QString{}});
}

void IniSettingsStoreShould::give_the_stored_values_after_a_restart()
{
    {
        auto store = IniSettingsStore{storePath()};
        store.setValue(switchKey, false);
        store.setValue(textKey, QStringLiteral("uuid:kitchen"));
    }

    auto const store = IniSettingsStore{storePath()};

    QCOMPARE(store.value(switchKey, true).toBool(), false);
    QCOMPARE(store.value(textKey), QVariant{QStringLiteral("uuid:kitchen")});
}

void IniSettingsStoreShould::forget_a_removed_key_across_a_restart()
{
    {
        auto store = IniSettingsStore{storePath()};
        store.setValue(switchKey, false);
        store.setValue(textKey, QStringLiteral("uuid:kitchen"));
        store.remove(textKey);
    }

    auto const store = IniSettingsStore{storePath()};

    QCOMPARE(store.value(switchKey, true).toBool(), false);
    QCOMPARE(store.value(textKey), QVariant{});
}

void IniSettingsStoreShould::give_the_defaults_when_the_file_is_missing()
{
    auto const store = IniSettingsStore{storePath()};

    QCOMPARE(store.value(switchKey, true).toBool(), true);
}

void IniSettingsStoreShould::give_the_defaults_when_the_file_is_corrupt()
{
    auto file = QFile{storePath()};
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("[shell\nswitch");
    file.close();

    auto const store = IniSettingsStore{storePath()};

    QCOMPARE(store.value(switchKey, true).toBool(), true);
}

void IniSettingsStoreShould::create_the_missing_directory_on_a_change()
{
    auto const path = mDir->filePath(QStringLiteral("blabby/settings.ini"));

    IniSettingsStore{path}.setValue(switchKey, false);

    QCOMPARE(IniSettingsStore{path}.value(switchKey, true).toBool(), false);
}

void IniSettingsStoreShould::keep_the_values_of_providers_apart_by_name()
{
    auto store = IniSettingsStore{storePath()};

    store.setValue(switchKey, true);
    store.setProviderValue(QStringLiteral("MediaServer"), switchKey, false);
    store.setProviderValue(QStringLiteral("UsbStick"), textKey, QStringLiteral("sda1"));

    QCOMPARE(store.value(switchKey), QVariant{true});
    QCOMPARE(store.providerValue(QStringLiteral("MediaServer"), switchKey), QVariant{false});
    QCOMPARE(store.providerValue(QStringLiteral("MediaServer"), textKey, QStringLiteral("none")),
             QVariant{QStringLiteral("none")});
    QCOMPARE(store.providerValue(QStringLiteral("UsbStick"), switchKey, true), QVariant{true});
    QCOMPARE(store.providerValue(QStringLiteral("UsbStick"), textKey), QVariant{QStringLiteral("sda1")});
}

void IniSettingsStoreShould::give_the_stored_provider_values_after_a_restart()
{
    {
        auto store = IniSettingsStore{storePath()};
        store.setProviderValue(QStringLiteral("MediaServer"), switchKey, false);
    }

    auto store = IniSettingsStore{storePath()};

    QCOMPARE(store.providerValue(QStringLiteral("MediaServer"), switchKey, true).toBool(), false);
}

} // namespace Shell

QTEST_MAIN(Shell::IniSettingsStoreShould)
