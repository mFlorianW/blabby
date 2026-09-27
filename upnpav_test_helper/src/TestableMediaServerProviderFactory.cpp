// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestableMediaServerProviderFactory.hpp"
#include "ClockDouble.hpp"
#include "DescriptionFetcherBackendDouble.hpp"
#include "ServiceDiscoveryBackendDouble.hpp"

namespace UPnPAV
{

std::unique_ptr<IServiceProvider> TestableMediaServerProviderFactory::createServiceProvider(QString const& searchTarget)
{
    auto serviceDiscoveryBackendDouble = std::make_unique<ServiceDiscoveryBackendDouble>();
    this->serviceDiscoveryBackendDouble = serviceDiscoveryBackendDouble.get();
    auto descriptionFetcherBackendDouble = std::make_unique<DescriptionFetcherBackendDouble>();
    this->descriptionFetcherBackendDouble = descriptionFetcherBackendDouble.get();
    auto clockDouble = std::make_unique<ClockDouble>();
    this->clockDouble = clockDouble.get();

    return std::make_unique<ServiceProvider>(searchTarget,
                                             std::move(serviceDiscoveryBackendDouble),
                                             std::move(descriptionFetcherBackendDouble),
                                             std::move(clockDouble));
}

} // namespace UPnPAV
