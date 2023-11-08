// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ServiceProviderDouble.h"
#include <QString>

namespace MediaServer::Plugin::Doubles
{

void ServiceProviderDouble::setSearchTarget(const QString &searchTarget) noexcept
{
    mSearchTarget = searchTarget;
}

void ServiceProviderDouble::startSearch() const noexcept
{
}

UPnPAV::DeviceDescription ServiceProviderDouble::rootDeviceDescription(const QString &usn) const noexcept
{
    Q_UNUSED(usn);
    return UPnPAV::DeviceDescription{};
}

std::unique_ptr<UPnPAV::IServiceProvider> ServiceProviderFactory::createServiceProvider(const QString &searchTarget)
{
    Q_UNUSED(searchTarget);
    auto sp = std::make_unique<ServiceProviderDouble>();
    serviceProvider = sp.get();
    return sp;
}

} // namespace MediaServer::Plugin::Doubles
