// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef TESTABLEMEDIASERVERPROVIDERFACTORY_h
#define TESTABLEMEDIASERVERPROVIDERFACTORY_h

#include "ServiceProvider.h"

namespace UPnPAV
{
class ServiceDiscoveryBackendDouble;
class DescriptionFetcherBackendDouble;

class TestableMediaServerProviderFactory final : public ServiceProviderFactory
{
public:
    std::unique_ptr<IServiceProvider> createServiceProvider(const QString &searchTarget) override;

    ServiceDiscoveryBackendDouble *serviceDiscoveryBackendDouble;
    DescriptionFetcherBackendDouble *descriptionFetcherBackendDouble;
};

} // namespace UPnPAV

#endif // TESTABLEMEDIASERVERPROVIDERFACTORY_h
