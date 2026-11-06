// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestProvider.hpp"
#include "TestSource.hpp"

namespace Multimedia::TestHelper
{

TestProvider::TestProvider(QString name)
    : Provider(std::move(name), Multimedia::ProviderVersion{.major = 0, .minor = 1, .patch = 0})
{
}

TestProvider::~TestProvider() = default;

bool TestProvider::init() noexcept
{
    if (mInitFails) {
        return false;
    }
    mSources.emplace_back(std::make_shared<TestSource>(QStringLiteral("TestMediaSource"),
                                                       QStringLiteral("http::/127.0.0.1/TestMediaSource.png")));
    return true;
}

Multimedia::MediaSources TestProvider::sources() const noexcept
{
    return mSources;
}

void TestProvider::createNewSource() noexcept
{
    auto src =
        mSources.emplace_back(std::make_shared<TestSource>(QStringLiteral("TestMediaSource"),
                                                           QStringLiteral("http::/127.0.0.1/TestMediaSource.png")));
    Q_EMIT sourceAdded(src);
}

void TestProvider::removeLastSource() noexcept
{
    if (not mSources.isEmpty()) {
        auto srcRemoved = mSources.at(mSources.size() - 1);
        mSources.removeLast();
        Q_EMIT sourceRemoved(srcRemoved);
    }
}

void TestProvider::setInitFails(bool initFails) noexcept
{
    mInitFails = initFails;
}

QString TestProvider::displayTitle() const noexcept
{
    return mDisplayTitle;
}

QList<Multimedia::ProviderSetting> TestProvider::settings() const noexcept
{
    return mSettings;
}

void TestProvider::setSettingValue(QString const& key, QVariant const& value) noexcept
{
    mPassedValues.append({key, value});
}

void TestProvider::triggerAction(QString const& key) noexcept
{
    mTriggeredActions.append(key);
}

void TestProvider::setDisplayTitle(QString displayTitle) noexcept
{
    mDisplayTitle = std::move(displayTitle);
}

void TestProvider::setSettings(QList<Multimedia::ProviderSetting> settings) noexcept
{
    mSettings = std::move(settings);
}

QList<std::pair<QString, QVariant>> const& TestProvider::passedValues() const noexcept
{
    return mPassedValues;
}

QStringList const& TestProvider::triggeredActions() const noexcept
{
    return mTriggeredActions;
}

} // namespace Multimedia::TestHelper
