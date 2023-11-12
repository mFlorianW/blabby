// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "MediaServer.h"
#include "Provider.h"
#include "ServiceProvider.h"
#include "blabbymediaserverprovider_export.h"
#include <memory>

namespace Provider
{

class BLABBYMEDIASERVERPROVIDER_EXPORT MediaServerProvider : public Multimedia::Provider
{
    Q_OBJECT
    Q_INTERFACES(Multimedia::Provider)
    Q_PLUGIN_METADATA(IID "org.blabby.Provider")
public:
    MediaServerProvider(
        std::unique_ptr<UPnPAV::IServiceProviderFactory> serviceProviderFab =
            std::make_unique<UPnPAV::ServiceProviderFactory>(),
        std::unique_ptr<UPnPAV::MediaServerFactory> mediaServerFab = std::make_unique<UPnPAV::MediaServerFactory>());
    ~MediaServerProvider() override;

    Q_DISABLE_COPY_MOVE(MediaServerProvider)

    bool init() noexcept override;

    QVector<std::shared_ptr<Multimedia::MediaSource>> sources() const noexcept override;

private Q_SLOTS:
    void onServiceConnected(QString const &usn) noexcept;
    void onServiceDisconnected(QString const &usn) noexcept;

private:
    std::unique_ptr<UPnPAV::IServiceProviderFactory> mServiceProviderFab;
    std::unique_ptr<UPnPAV::MediaServerFactory> mMediaServerFab;
    std::unique_ptr<UPnPAV::IServiceProvider> mServiceProvider;
    QHash<QString, std::shared_ptr<Multimedia::MediaSource>> mMediaServers;
};

} // namespace Provider
