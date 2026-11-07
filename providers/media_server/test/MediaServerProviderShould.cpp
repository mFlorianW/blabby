// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerProviderShould.hpp"
#include "MediaServerDouble.hpp"
#include "MediaServerProvider.hpp"
#include "ServiceProviderDouble.hpp"
#include <QSignalSpy>
#include <QTest>

namespace Provider::MediaServer
{

namespace
{
constexpr auto rescanNetworkKey = QLatin1StringView{"rescanNetwork"};
} // namespace

ProviderShould::~ProviderShould() = default;

void ProviderShould::send_find_request_for_media_server_on_init()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto msFab = std::make_unique<UPnPAV::Doubles::MediaServerFactory>();
    auto prov = Provider{std::move(spFab), std::move(msFab)};
    auto expSearchTarget = QStringLiteral("urn:schemas-upnp-org:device:MediaServer:1");

    prov.init();
    auto const& st = spFabRaw->serviceProvider->searchTarget();

    QVERIFY2(spFabRaw->serviceProvider != nullptr,
             QStringLiteral("The MediaServerProvider should create a ServiceProvider instance").toLocal8Bit());
    QVERIFY2(st == expSearchTarget,
             QString{"The searchTarget \"%1\"is not the expected one %2"}.arg(st, expSearchTarget).toLocal8Bit());
    QCOMPARE(spFabRaw->serviceProvider->isSearchTriggered(), true);
}

void ProviderShould::notify_when_a_server_appears()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto msFab = std::make_unique<UPnPAV::Doubles::MediaServerFactory>();
    auto prov = Provider{std::move(spFab), std::move(msFab)};
    auto spy = QSignalSpy{&prov, &Provider::sourceAdded};

    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));

    QVERIFY2(spy.size() == 1, "The MediaServerProvider should emit the signal source added.");
    QVERIFY2(spy.at(0).at(0).value<std::shared_ptr<Multimedia::Source>>() != nullptr,
             "The MediaSourceProvider sourceAdded signal should have a valid instance of the "
             "Multimedia::MediaSource.");
    QVERIFY2(prov.sources().isEmpty() == false, "The returned MediaSourcen shoudn't be empty.");
}

void ProviderShould::notify_when_a_server_disappears()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto msFab = std::make_unique<UPnPAV::Doubles::MediaServerFactory>();
    auto prov = Provider{std::move(spFab), std::move(msFab)};
    auto spy = QSignalSpy{&prov, &Provider::sourceRemoved};

    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));

    Q_EMIT spFabRaw->serviceProvider->serviceDisconnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));
    QVERIFY2(spy.size() == 1, "The MediaServerProvider should emit the signal source removed.");
    QVERIFY2(spy.at(0).at(0).value<std::shared_ptr<Multimedia::Source>>() != nullptr,
             "The MediaSourceProvider sourceRemoved signal should have a valid instance of the "
             "Multimedia::MediaSource.");
    QVERIFY2(prov.sources().isEmpty() == true, "The returned MediaSourcen shoudn't be empty.");
}

void ProviderShould::describe_a_rescan_network_action_under_media_servers()
{
    auto prov = Provider{std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>(),
                         std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};

    auto const settings = prov.settings();

    QCOMPARE(prov.displayTitle(), QStringLiteral("Media servers"));
    QCOMPARE(settings.size(), 1);
    QCOMPARE(settings.at(0).key, QString{rescanNetworkKey});
    QCOMPARE(settings.at(0).title, QStringLiteral("Rescan network"));
    QCOMPARE(settings.at(0).kind, Multimedia::ProviderSetting::Kind::Action);
}

void ProviderShould::search_the_network_again_on_rescan()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto prov = Provider{std::move(spFab), std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};
    prov.init();

    prov.triggerAction(rescanNetworkKey);

    QCOMPARE(spFabRaw->serviceProvider->searchCount(), 2);
}

void ProviderShould::ignore_unknown_actions()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto prov = Provider{std::move(spFab), std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};
    prov.init();

    prov.triggerAction(QStringLiteral("unknown"));

    QCOMPARE(spFabRaw->serviceProvider->searchCount(), 1);
}

void ProviderShould::report_servers_found_by_a_rescan_as_added()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto prov = Provider{std::move(spFab), std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};
    auto addedSpy = QSignalSpy{&prov, &Provider::sourceAdded};
    auto removedSpy = QSignalSpy{&prov, &Provider::sourceRemoved};
    prov.init();
    Q_EMIT spFabRaw->serviceProvider->searchFinished();

    prov.triggerAction(rescanNetworkKey);
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:server-a"));
    Q_EMIT spFabRaw->serviceProvider->searchFinished();

    QCOMPARE(addedSpy.size(), 1);
    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(prov.sources().size(), 1);
}

void ProviderShould::report_servers_missing_after_a_rescan_as_removed()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto prov = Provider{std::move(spFab), std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};
    auto addedSpy = QSignalSpy{&prov, &Provider::sourceAdded};
    auto removedSpy = QSignalSpy{&prov, &Provider::sourceRemoved};
    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:server-a"));
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:server-b"));
    Q_EMIT spFabRaw->serviceProvider->searchFinished();
    auto const serverA = addedSpy.at(0).at(0).value<std::shared_ptr<Multimedia::Source>>();
    auto const serverB = addedSpy.at(1).at(0).value<std::shared_ptr<Multimedia::Source>>();

    prov.triggerAction(rescanNetworkKey);
    Q_EMIT spFabRaw->serviceProvider->serviceSeen(QStringLiteral("uuid:server-a"));
    QCOMPARE(removedSpy.size(), 0);
    Q_EMIT spFabRaw->serviceProvider->searchFinished();

    QCOMPARE(removedSpy.size(), 1);
    QCOMPARE(removedSpy.at(0).at(0).value<std::shared_ptr<Multimedia::Source>>(), serverB);
    QCOMPARE(spFabRaw->serviceProvider->disconnectedServices(), QStringList{QStringLiteral("uuid:server-b")});
    QCOMPARE(prov.sources(), Multimedia::MediaSources{serverA});
}

void ProviderShould::remove_servers_only_at_the_end_of_a_rescan()
{
    auto spFab = std::make_unique<UPnPAV::Doubles::ServiceProviderFactory>();
    auto* spFabRaw = spFab.get();
    auto prov = Provider{std::move(spFab), std::make_unique<UPnPAV::Doubles::MediaServerFactory>()};
    auto removedSpy = QSignalSpy{&prov, &Provider::sourceRemoved};
    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:server-a"));

    Q_EMIT spFabRaw->serviceProvider->searchFinished();
    prov.triggerAction(rescanNetworkKey);
    Q_EMIT spFabRaw->serviceProvider->serviceSeen(QStringLiteral("uuid:server-a"));
    Q_EMIT spFabRaw->serviceProvider->searchFinished();
    Q_EMIT spFabRaw->serviceProvider->searchFinished();

    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(prov.sources().size(), 1);
}

} // namespace Provider::MediaServer

QTEST_MAIN(Provider::MediaServer::ProviderShould)
