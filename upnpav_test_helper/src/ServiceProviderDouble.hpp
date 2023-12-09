// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "IServiceProvider.hpp"

namespace UPnPAV::Doubles
{

class ServiceProviderDouble final : public UPnPAV::IServiceProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderDouble)
public:
    ServiceProviderDouble(QString searchTarget = QString(""));
    ~ServiceProviderDouble() noexcept override = default;

    void setSearchTarget(const QString &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    UPnPAV::DeviceDescription rootDeviceDescription(const QString &usn) const noexcept override;

    QString const &searchTarget() const noexcept;

    bool isSearchTriggered() const noexcept;

private:
    QString mSearchTarget;
    mutable bool mSearchRequested{false};
};

class ServiceProviderFactory : public UPnPAV::IServiceProviderFactory
{
public:
    ServiceProviderDouble *serviceProvider{nullptr};
    std::unique_ptr<UPnPAV::IServiceProvider> createServiceProvider(const QString &searchTarget) override;
};

} // namespace UPnPAV::Doubles
