// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerProvider.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaServerSource.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <source_location>

namespace Provider
{

MediaServerProvider::MediaServerProvider(std::unique_ptr<UPnPAV::IServiceProviderFactory> serviceProviderFab,
                                         std::unique_ptr<UPnPAV::MediaServerFactory> mediaServerFab)
    : Provider{QStringLiteral("MediaServerProvider"), Multimedia::ProviderVersion{.major = 0, .minor = 1, .patch = 1}}
    , mServiceProviderFab{std::move(serviceProviderFab)}
    , mMediaServerFab{std::move(mediaServerFab)}
{
}

MediaServerProvider::~MediaServerProvider() = default;

bool MediaServerProvider::init() noexcept
{
    Q_ASSERT(mServiceProviderFab != nullptr);

    mServiceProvider =
        mServiceProviderFab->createServiceProvider(QStringLiteral("urn:schemas-upnp-org:device:MediaServer:1"));

    connect(mServiceProvider.get(),
            &UPnPAV::IServiceProvider::serviceConnected,
            this,
            &MediaServerProvider::onServiceConnected);
    connect(mServiceProvider.get(),
            &UPnPAV::IServiceProvider::serviceDisconnected,
            this,
            &MediaServerProvider::onServiceDisconnected);

    mServiceProvider->startSearch();
    return true;
}

QVector<std::shared_ptr<Multimedia::MediaSource>> MediaServerProvider::sources() const noexcept
{
    auto result = QVector<std::shared_ptr<Multimedia::MediaSource>>{};
    auto iter = QHashIterator<QString, std::shared_ptr<Multimedia::MediaSource>>{mMediaServers};
    while (iter.hasNext())
    {
        iter.next();
        result.push_back(iter.value());
    }
    return result;
}

void MediaServerProvider::onServiceConnected(QString const &usn) noexcept
{
    Q_ASSERT(mServiceProvider != nullptr);
    Q_ASSERT(mMediaServerFab != nullptr);

    const auto devDesc = mServiceProvider->rootDeviceDescription(usn);
    try
    {
        auto ms = mMediaServerFab->createMediaServer(devDesc);
        auto msp = std::make_shared<MediaServerSource>(std::move(ms));
        mMediaServers.insert(usn, msp);
        Q_EMIT sourceAdded(msp);
    }
    catch (UPnPAV::InvalidDeviceDescription &invDesc)
    {
        qCCritical(mediaServerProvider()) << "Failed to create MediaServer. Error:" << invDesc.what();
    }
}

void MediaServerProvider::onServiceDisconnected(QString const &usn) noexcept
{
    if (!mMediaServers.contains(usn))
    {
        qCDebug(mediaServerProvider()) << "Can't remove media server" << usn << "not found.";
        return;
    }

    auto msp = mMediaServers.value(usn);
    Q_EMIT sourceRemoved(msp);
    mMediaServers.remove(usn);
}

} // namespace Provider
