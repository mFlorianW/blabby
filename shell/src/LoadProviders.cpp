// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "LoadProviders.hpp"
#include "LoggingCategories.hpp"

namespace Shell
{

Multimedia::Providers loadProviders(Multimedia::ProviderLoader& loader, QStringList const& paths)
{
    auto providers = Multimedia::Providers{};
    for (auto const& provider : loader.load(paths)) {
        if (provider->init()) {
            providers.append(provider);
        } else {
            qCWarning(shell) << "Failed to initialize Provider:" << provider->name();
        }
    }
    return providers;
}

} // namespace Shell
