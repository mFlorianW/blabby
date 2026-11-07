// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaServer.hpp"
#include "Provider.hpp"
#include "ServiceProvider.hpp"
#include "blabbymediaserverprovider_export.h"
#include <QSet>
#include <memory>
#include <optional>

namespace Provider::MediaServer
{

/**
 * MediaServerProvider searches the local Network for MediaServers and provides the
 * access to them by @ref Multimedia::MediaSource interface.
 */
class BLABBYMEDIASERVERPROVIDER_EXPORT Provider : public Multimedia::Provider
{
    Q_OBJECT
    Q_INTERFACES(Multimedia::Provider)
    Q_PLUGIN_METADATA(IID "org.blabby.Provider")
public:
    /**
     * Creates an instance of the media server provider
     * @param serviceProviderFab The factory to create the service provider. Default @ref UPnPAV::ServiceProviderFactory
     * @param mediaServerFab The factory that shall be used when a creating MediaServer
     */
    Provider(
        std::unique_ptr<UPnPAV::IServiceProviderFactory> serviceProviderFab =
            std::make_unique<UPnPAV::ServiceProviderFactory>(),
        std::unique_ptr<UPnPAV::MediaServerFactory> mediaServerFab = std::make_unique<UPnPAV::MediaServerFactory>());

    /**
     * Destructor
     */
    ~Provider() override;

    /**
     * Disabled copy and move operators
     */
    Q_DISABLE_COPY_MOVE(Provider)

    /**
     * Basic init of the MediaServerProvider
     * @return always true.
     */
    bool init() noexcept override;

    /**
     * @copydoc Multimedia::MediaSource::sources
     */
    QVector<std::shared_ptr<Multimedia::Source>> sources() const noexcept override;

    /**
     * @copydoc Multimedia::Provider::displayTitle
     */
    QString displayTitle() const noexcept override;

    /**
     * Gives the "Rescan network" action.
     * @copydoc Multimedia::Provider::settings
     */
    QList<Multimedia::ProviderSetting> settings() const noexcept override;

    /**
     * Rescans the network for the "Rescan network" action.
     * @copydoc Multimedia::Provider::triggerAction
     */
    void triggerAction(QString const& key) noexcept override;

private Q_SLOTS:
    void onServiceConnected(QString const& usn) noexcept;
    void onServiceDisconnected(QString const& usn) noexcept;
    void onServiceSeen(QString const& usn) noexcept;
    void onSearchFinished() noexcept;

private:
    /**
     * Searches the network for MediaServers again, the ones that don't answer until the search finished are removed.
     */
    void rescan() noexcept;

    std::unique_ptr<UPnPAV::IServiceProviderFactory> mServiceProviderFab;
    std::unique_ptr<UPnPAV::MediaServerFactory> mMediaServerFab;
    std::unique_ptr<UPnPAV::IServiceProvider> mServiceProvider;
    QHash<QString, std::shared_ptr<Multimedia::Source>> mMediaServers;
    std::optional<QSet<QString>> mUnansweredMediaServers;
};

} // namespace Provider::MediaServer
