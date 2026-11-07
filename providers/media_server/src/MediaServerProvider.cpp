// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerProvider.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaServerSource.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>

namespace Provider::MediaServer
{

namespace
{
constexpr auto rescanNetworkKey = QLatin1StringView{"rescanNetwork"};
} // namespace

Provider::Provider(std::unique_ptr<UPnPAV::IServiceProviderFactory> serviceProviderFab,
                   std::unique_ptr<UPnPAV::MediaServerFactory> mediaServerFab)
    : Multimedia::Provider{QStringLiteral("MediaServerProvider"),
                           Multimedia::ProviderVersion{.major = 0, .minor = 1, .patch = 1}}
    , mServiceProviderFab{std::move(serviceProviderFab)}
    , mMediaServerFab{std::move(mediaServerFab)}
{
}

Provider::~Provider() = default;

bool Provider::init() noexcept
{
    Q_ASSERT(mServiceProviderFab != nullptr);

    mServiceProvider =
        mServiceProviderFab->createServiceProvider(QStringLiteral("urn:schemas-upnp-org:device:MediaServer:1"));

    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::serviceConnected, this, &Provider::onServiceConnected);
    connect(mServiceProvider.get(),
            &UPnPAV::IServiceProvider::serviceDisconnected,
            this,
            &Provider::onServiceDisconnected);
    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::serviceSeen, this, &Provider::onServiceSeen);
    connect(mServiceProvider.get(), &UPnPAV::IServiceProvider::searchFinished, this, &Provider::onSearchFinished);

    mServiceProvider->startSearch();
    return true;
}

QVector<std::shared_ptr<Multimedia::Source>> Provider::sources() const noexcept
{
    auto result = QVector<std::shared_ptr<Multimedia::Source>>{};
    auto iter = QHashIterator<QString, std::shared_ptr<Multimedia::Source>>{mMediaServers};
    while (iter.hasNext()) {
        iter.next();
        result.push_back(iter.value());
    }
    return result;
}

QString Provider::displayTitle() const noexcept
{
    return tr("Media servers");
}

QList<Multimedia::ProviderSetting> Provider::settings() const noexcept
{
    return {Multimedia::ProviderSetting{.key = rescanNetworkKey,
                                        .title = tr("Rescan network"),
                                        .subtitle = tr("Search the network for media servers again"),
                                        .kind = Multimedia::ProviderSetting::Kind::Action}};
}

void Provider::triggerAction(QString const& key) noexcept
{
    if (key == rescanNetworkKey) {
        rescan();
    }
}

void Provider::rescan() noexcept
{
    Q_ASSERT(mServiceProvider != nullptr);

    qCDebug(mediaServerProvider()) << "Rescan the network for MediaServers.";
    auto const knownMediaServers = mMediaServers.keys();
    mUnansweredMediaServers = QSet<QString>{knownMediaServers.cbegin(), knownMediaServers.cend()};
    mServiceProvider->startSearch();
}

void Provider::onServiceConnected(QString const& usn) noexcept
{
    Q_ASSERT(mServiceProvider != nullptr);
    Q_ASSERT(mMediaServerFab != nullptr);

    auto const devDesc = mServiceProvider->rootDeviceDescription(usn);
    try {
        auto ms = mMediaServerFab->createMediaServer(devDesc);
        auto msp = std::make_shared<Source>(std::move(ms));
        mMediaServers.insert(usn, msp);
        Q_EMIT sourceAdded(msp);
    } catch (UPnPAV::InvalidDeviceDescription& invDesc) {
        qCCritical(mediaServerProvider()) << "Failed to create MediaServer. Error:" << invDesc.what();
    }
}

void Provider::onServiceDisconnected(QString const& usn) noexcept
{
    if (!mMediaServers.contains(usn)) {
        qCDebug(mediaServerProvider()) << "Can't remove media server" << usn << "not found.";
        return;
    }

    auto msp = mMediaServers.value(usn);
    Q_EMIT sourceRemoved(msp);
    mMediaServers.remove(usn);
    if (mUnansweredMediaServers.has_value()) {
        mUnansweredMediaServers->remove(usn);
    }
}

void Provider::onServiceSeen(QString const& usn) noexcept
{
    if (mUnansweredMediaServers.has_value()) {
        mUnansweredMediaServers->remove(usn);
    }
}

void Provider::onSearchFinished() noexcept
{
    if (not mUnansweredMediaServers.has_value()) {
        return;
    }

    // Disconnecting them in the service provider lets their next announcement connect them again.
    auto const unansweredMediaServers = *mUnansweredMediaServers;
    mUnansweredMediaServers.reset();
    for (auto const& usn : unansweredMediaServers) {
        qCDebug(mediaServerProvider()) << "MediaServer" << usn << "didn't answer the rescan.";
        mServiceProvider->disconnectService(usn);
    }
}

} // namespace Provider::MediaServer
