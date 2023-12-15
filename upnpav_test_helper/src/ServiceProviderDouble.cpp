// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ServiceProviderDouble.hpp"
#include <QString>

namespace UPnPAV::Doubles
{

ServiceProviderDouble::ServiceProviderDouble(QString searchTarget)
    : IServiceProvider{}
    , mSearchTarget{std::move(searchTarget)}
{
}

void ServiceProviderDouble::setSearchTarget(const QString &searchTarget) noexcept
{
    mSearchTarget = searchTarget;
}

void ServiceProviderDouble::startSearch() const noexcept
{
    mSearchRequested = true;
}

UPnPAV::DeviceDescription ServiceProviderDouble::rootDeviceDescription(const QString &usn) const noexcept
{
    if (mDevices.contains(usn)) {
        return mDevices.value(usn);
    }
    return UPnPAV::DeviceDescription{};
}

void ServiceProviderDouble::addDeviceDescription(QString const &usn, UPnPAV::DeviceDescription const &desc) noexcept
{
    mDevices.insert(usn, desc);
}

QString const &ServiceProviderDouble::searchTarget() const noexcept
{
    return mSearchTarget;
}

bool ServiceProviderDouble::isSearchTriggered() const noexcept
{
    return mSearchRequested;
}

std::unique_ptr<UPnPAV::IServiceProvider> ServiceProviderFactory::createServiceProvider(const QString &searchTarget)
{
    auto sp = std::make_unique<ServiceProviderDouble>(searchTarget);
    serviceProvider = sp.get();
    return sp;
}

} // namespace UPnPAV::Doubles
