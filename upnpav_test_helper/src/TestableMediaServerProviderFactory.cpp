// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "TestableMediaServerProviderFactory.h"
#include "DescriptionFetcherBackendDouble.h"
#include "ServiceDiscoveryBackendDouble.h"

namespace UPnPAV
{

std::unique_ptr<IServiceProvider> TestableMediaServerProviderFactory::createServiceProvider(const QString &searchTarget)
{
    auto serviceDiscoveryBackendDouble = std::make_unique<ServiceDiscoveryBackendDouble>();
    this->serviceDiscoveryBackendDouble = serviceDiscoveryBackendDouble.get();
    auto descriptionFetcherBackendDouble = std::make_unique<DescriptionFetcherBackendDouble>();
    this->descriptionFetcherBackendDouble = descriptionFetcherBackendDouble.get();

    return std::make_unique<ServiceProvider>(searchTarget,
                                             std::move(serviceDiscoveryBackendDouble),
                                             std::move(descriptionFetcherBackendDouble));
}

} // namespace UPnPAV
