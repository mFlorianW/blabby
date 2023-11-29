// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "TestProviderLoader.hpp"
#include "TestProvider.hpp"

namespace Multimedia::TestHelper
{

TestProviderLoader::TestProviderLoader(std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        mProviders.emplace_back(
            std::make_shared<TestProvider>(QStringLiteral("TestProvider%1").arg(QString::number(i))));
    }
}

[[nodiscard]] Multimedia::Providers const &TestProviderLoader::load(QStringList const &paths) noexcept
{
    Q_UNUSED(paths)
    return mProviders;
}

const Multimedia::Providers &TestProviderLoader::providers() const noexcept
{
    return mProviders;
}

} // namespace Multimedia::TestHelper
