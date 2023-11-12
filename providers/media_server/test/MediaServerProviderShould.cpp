// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "MediaServerProviderShould.h"
#include "MediaServerDouble.h"
#include "MediaServerProvider.h"
#include "ServiceProviderDouble.h"
#include <QSignalSpy>
#include <QTest>

namespace Provider
{

MediaServerProviderShould::~MediaServerProviderShould() = default;

void MediaServerProviderShould::send_find_request_for_media_server_on_init()
{
    auto spFab = std::make_unique<Doubles::ServiceProviderFactory>();
    auto *spFabRaw = spFab.get();
    auto msFab = std::make_unique<Doubles::MediaServerFactory>();
    auto prov = MediaServerProvider{std::move(spFab), std::move(msFab)};
    auto expSearchTarget = QStringLiteral("urn:schemas-upnp-org:device:MediaServer:1");

    prov.init();
    const auto &st = spFabRaw->serviceProvider->searchTarget();

    QVERIFY2(spFabRaw->serviceProvider != nullptr,
             QStringLiteral("The MediaServerProvider should create a ServiceProvider instance").toLocal8Bit());
    QVERIFY2(st == expSearchTarget,
             QString{"The searchTarget \"%1\"is not the expected one %2"}.arg(st, expSearchTarget).toLocal8Bit());
    QCOMPARE(spFabRaw->serviceProvider->isSearchTriggered(), true);
}

void MediaServerProviderShould::notify_when_a_server_appears()
{
    auto spFab = std::make_unique<Doubles::ServiceProviderFactory>();
    auto *spFabRaw = spFab.get();
    auto msFab = std::make_unique<Doubles::MediaServerFactory>();
    auto prov = MediaServerProvider{std::move(spFab), std::move(msFab)};
    auto spy = QSignalSpy{&prov, &MediaServerProvider::sourceAdded};

    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));

    QVERIFY2(spy.size() == 1, "The MediaServerProvider should emit the signal source added.");
    QVERIFY2(spy.at(0).at(0).value<std::shared_ptr<Multimedia::MediaSource>>() != nullptr,
             "The MediaSourceProvider sourceAdded signal should have a valid instance of the "
             "Multimedia::MediaSource.");
    QVERIFY2(prov.sources().isEmpty() == false, "The returned MediaSourcen shoudn't be empty.");
}

void MediaServerProviderShould::notify_when_a_server_disappears()
{
    auto spFab = std::make_unique<Doubles::ServiceProviderFactory>();
    auto *spFabRaw = spFab.get();
    auto msFab = std::make_unique<Doubles::MediaServerFactory>();
    auto prov = MediaServerProvider{std::move(spFab), std::move(msFab)};
    auto spy = QSignalSpy{&prov, &MediaServerProvider::sourceRemoved};

    prov.init();
    Q_EMIT spFabRaw->serviceProvider->serviceConnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));

    Q_EMIT spFabRaw->serviceProvider->serviceDisconnected(QStringLiteral("uuid:4d696e69-444c-164e-9d41-b827eb54e939"));
    QVERIFY2(spy.size() == 1, "The MediaServerProvider should emit the signal source removed.");
    QVERIFY2(spy.at(0).at(0).value<std::shared_ptr<Multimedia::MediaSource>>() != nullptr,
             "The MediaSourceProvider sourceRemoved signal should have a valid instance of the "
             "Multimedia::MediaSource.");
    QVERIFY2(prov.sources().isEmpty() == true, "The returned MediaSourcen shoudn't be empty.");
}

} // namespace Provider

QTEST_MAIN(Provider::MediaServerProviderShould)
