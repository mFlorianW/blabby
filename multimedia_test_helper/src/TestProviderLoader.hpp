// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ProviderLoader.hpp"

namespace Multimedia::TestHelper
{
class TestProviderLoader : public Multimedia::ProviderLoader
{
public:
    TestProviderLoader(std::size_t count = 1);
    [[nodiscard]] Multimedia::Providers const& load(QStringList const& paths) noexcept override;

    Multimedia::Providers const& providers() const noexcept;

private:
    Multimedia::Providers mProviders;
};
} // namespace Multimedia::TestHelper
