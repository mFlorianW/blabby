// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ProviderLoader.hpp"

namespace Shell
{

/**
 * Loads the Providers in the passed folders and initializes each of them once. The shell loads the Providers once at
 * startup and shares the returned list with every part that works with them. A Provider is initialized before anyone
 * connects to it, so it reports the Sources it has after initializing through @ref Multimedia::Provider::sources, not
 * through @ref Multimedia::Provider::sourceAdded.
 * @param loader The loader that loads the Provider plugins.
 * @param paths The folders that are searched for Provider plugins.
 * @return The loaded Providers that initialized, a Provider that failed to initialize is left out.
 */
Multimedia::Providers loadProviders(Multimedia::ProviderLoader& loader, QStringList const& paths);

} // namespace Shell
