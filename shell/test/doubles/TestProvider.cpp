// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "TestProvider.hpp"
#include "TestMediaSource.hpp"

namespace Shell::Doubles
{

TestProvider::TestProvider(QString name)
    : Provider(std::move(name), Multimedia::ProviderVersion{.major = 0, .minor = 1, .patch = 0})
{
}

TestProvider::~TestProvider() = default;

bool TestProvider::init() noexcept
{
    mSources.emplace_back(std::make_shared<TestMediaSource>(QStringLiteral("TestMediaSource"),
                                                            QStringLiteral("http::/127.0.0.1/TestMediaSource.png")));
    return true;
}

Multimedia::MediaSources TestProvider::sources() const noexcept
{
    return mSources;
}

void TestProvider::createNewSource() noexcept
{
    auto src = mSources.emplace_back(
        std::make_shared<TestMediaSource>(QStringLiteral("TestMediaSource"),
                                          QStringLiteral("http::/127.0.0.1/TestMediaSource.png")));
    Q_EMIT sourceAdded(src);
}

void TestProvider::removeLastSource() noexcept
{
    if (not mSources.isEmpty())
    {
        auto srcRemoved = mSources.at(mSources.size() - 1);
        mSources.removeLast();
        Q_EMIT sourceRemoved(srcRemoved);
    }
}

} // namespace Shell::Doubles
