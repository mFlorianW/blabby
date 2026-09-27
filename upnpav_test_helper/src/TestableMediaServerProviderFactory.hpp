// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef TESTABLEMEDIASERVERPROVIDERFACTORY_h
#define TESTABLEMEDIASERVERPROVIDERFACTORY_h

#include "ServiceProvider.hpp"

namespace UPnPAV
{
class ServiceDiscoveryBackendDouble;
class DescriptionFetcherBackendDouble;
class ClockDouble;

class TestableMediaServerProviderFactory final : public ServiceProviderFactory
{
public:
    std::unique_ptr<IServiceProvider> createServiceProvider(QString const& searchTarget) override;

    ServiceDiscoveryBackendDouble* serviceDiscoveryBackendDouble{nullptr};
    DescriptionFetcherBackendDouble* descriptionFetcherBackendDouble{nullptr};
    ClockDouble* clockDouble{nullptr};
};

} // namespace UPnPAV

#endif // TESTABLEMEDIASERVERPROVIDERFACTORY_h
