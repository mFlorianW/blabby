// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "IServiceProvider.hpp"
#include <QHash>

namespace UPnPAV::Doubles
{

class ServiceProviderDouble final : public UPnPAV::IServiceProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceProviderDouble)
public:
    ServiceProviderDouble(QString searchTarget = QString(""));
    ~ServiceProviderDouble() noexcept override = default;

    void setSearchTarget(QString const &searchTarget) noexcept override;

    void startSearch() const noexcept override;

    UPnPAV::DeviceDescription rootDeviceDescription(QString const &usn) const noexcept override;
    void addDeviceDescription(QString const &usn, UPnPAV::DeviceDescription const &desc) noexcept;

    QString const &searchTarget() const noexcept;

    bool isSearchTriggered() const noexcept;

private:
    QString mSearchTarget;
    mutable bool mSearchRequested{false};
    QHash<QString, UPnPAV::DeviceDescription> mDevices;
};

class ServiceProviderFactory : public UPnPAV::IServiceProviderFactory
{
public:
    ServiceProviderDouble *serviceProvider{nullptr};
    std::unique_ptr<UPnPAV::IServiceProvider> createServiceProvider(QString const &searchTarget) override;
};

} // namespace UPnPAV::Doubles
