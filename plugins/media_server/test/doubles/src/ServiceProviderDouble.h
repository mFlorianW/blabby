// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "IServiceProvider.h"

namespace MediaServer::Plugin::Doubles
{


class ServiceProviderDouble final : public UPnPAV::IServiceProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderDouble)
public:
    ServiceProviderDouble() = default;
    ~ServiceProviderDouble() noexcept override = default;

    void setSearchTarget(const QString &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    UPnPAV::DeviceDescription rootDeviceDescription(const QString &usn) const noexcept;

private:
    QString mSearchTarget;
};

class ServiceProviderFactory : public UPnPAV::IServiceProviderFactory
{
public:
    ServiceProviderDouble *serviceProvider{ nullptr };
    std::unique_ptr<UPnPAV::IServiceProvider> createServiceProvider(const QString &searchTarget) override;
};

} // namespace MediaServer::Plugin::Doubles
