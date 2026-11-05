// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "LoadProvidersShould.hpp"
#include "LoadProviders.hpp"
#include "TestProvider.hpp"
#include "TestProviderLoader.hpp"
#include <QTest>

namespace Shell
{

LoadProvidersShould::~LoadProvidersShould() = default;

void LoadProvidersShould::give_every_provider_that_initialized()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};

    auto const providers = loadProviders(loader, {});

    QCOMPARE(providers, loader.providers());
}

void LoadProvidersShould::leave_out_a_provider_that_failed_to_initialize()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{2};
    qobject_cast<Multimedia::TestHelper::TestProvider*>(loader.providers().at(0).get())->setInitFails(true);

    auto const providers = loadProviders(loader, {});

    QCOMPARE(providers, Multimedia::Providers{loader.providers().at(1)});
}

void LoadProvidersShould::initialize_every_provider_once()
{
    auto loader = Multimedia::TestHelper::TestProviderLoader{};

    auto const providers = loadProviders(loader, {});

    // The test Provider adds a Source every time it is initialized.
    QCOMPARE(providers.at(0)->sources().size(), 1);
}

} // namespace Shell

QTEST_MAIN(Shell::LoadProvidersShould);
