// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RendererProviderShould.hpp"
#include "Descriptions.hpp"
#include "InMemoryRendererStore.hpp"
#include "MediaRendererDoubleFactory.hpp"
#include "RendererProvider.hpp"
#include "ServiceProviderDouble.hpp"
#include <QSignalSpy>
#include <QTest>

namespace Multimedia
{

RendererProviderShould::~RendererProviderShould() = default;

void RendererProviderShould::send_find_request_for_media_renderer_on_discover()
{
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    auto sProvRaw = sProv.get();
    auto prov = RendererProvider{std::make_shared<TestHelper::InMemoryRendererStore>(), std::move(sProv)};

    prov.discover();

    QCOMPARE(sProvRaw->searchTarget(), QStringLiteral("urn:schemas-upnp-org:device:MediaRenderer:1"));
    QCOMPARE(sProvRaw->isSearchTriggered(), true);
}

void RendererProviderShould::inform_about_connected_renderer_and_give_the_renderer()
{
    auto const usn = QStringLiteral("uuid:3f5de139-3457-4cf4-8190-e05f069bc803");
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    sProv->addDeviceDescription(usn, UPnPAV::validRendererDeviceDescription());
    auto sProvRaw = sProv.get();
    auto prov = RendererProvider{std::make_shared<TestHelper::InMemoryRendererStore>(), std::move(sProv)};
    auto rendererConnectSpy = QSignalSpy{&prov, &RendererProvider::rendererConnected};

    Q_EMIT sProvRaw->serviceConnected(usn);

    QCOMPARE(rendererConnectSpy.size(), 1);
    QCOMPARE_NE(rendererConnectSpy.at(0).at(0).value<std::shared_ptr<Renderer>>(), nullptr);
}

void RendererProviderShould::inform_about_disconnectd_renderer()
{
    auto const usn = QStringLiteral("uuid:3f5de139-3457-4cf4-8190-e05f069bc803");
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    sProv->addDeviceDescription(usn, UPnPAV::validRendererDeviceDescription());
    auto sProvRaw = sProv.get();
    auto prov = RendererProvider{std::make_shared<TestHelper::InMemoryRendererStore>(), std::move(sProv)};
    auto rendererDisconnectSpy = QSignalSpy{&prov, &RendererProvider::rendererDisconnected};

    Q_EMIT sProvRaw->serviceConnected(usn);
    Q_EMIT sProvRaw->serviceDisconnected(usn);
    QCOMPARE(rendererDisconnectSpy.size(), 1);
    QCOMPARE_NE(rendererDisconnectSpy.at(0).at(0).value<std::shared_ptr<Renderer>>(), nullptr);
}

void RendererProviderShould::inform_about_the_end_of_a_discovery()
{
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    auto sProvRaw = sProv.get();
    auto prov = RendererProvider{std::make_shared<TestHelper::InMemoryRendererStore>(), std::move(sProv)};
    auto discoveryFinishedSpy = QSignalSpy{&prov, &RendererProvider::discoveryFinished};
    prov.discover();

    Q_EMIT sProvRaw->searchFinished();

    QCOMPARE(discoveryFinishedSpy.size(), 1);
}

void RendererProviderShould::save_the_remembered_renderers_only_when_they_changed()
{
    auto const usn = QStringLiteral("uuid:kitchen");
    auto store = std::make_shared<TestHelper::InMemoryRendererStore>();
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    sProv->addDeviceDescription(usn,
                                UPnPAV::validRendererDeviceDescription(QStringLiteral("Kitchen"),
                                                                       QString{},
                                                                       QString{},
                                                                       usn,
                                                                       QStringLiteral("192.168.1.42")));
    auto* sProvRaw = sProv.get();
    auto prov = RendererProvider{store, std::move(sProv)};

    Q_EMIT sProvRaw->serviceConnected(usn);
    QCOMPARE(store->saveCount(), 1);

    Q_EMIT sProvRaw->serviceDisconnected(usn);
    Q_EMIT sProvRaw->serviceConnected(usn);
    QCOMPARE(store->saveCount(), 1);

    sProvRaw->addDeviceDescription(usn,
                                   UPnPAV::validRendererDeviceDescription(QStringLiteral("Kitchen"),
                                                                          QString{},
                                                                          QString{},
                                                                          usn,
                                                                          QStringLiteral("192.168.1.43")));
    Q_EMIT sProvRaw->serviceDisconnected(usn);
    Q_EMIT sProvRaw->serviceConnected(usn);
    QCOMPARE(store->saveCount(), 2);
    QCOMPARE(store->load().at(0).address, QStringLiteral("192.168.1.43"));
}

void RendererProviderShould::bring_a_renderer_whose_device_did_not_answer_online_again_on_its_next_announcement()
{
    auto const usn = QStringLiteral("uuid:kitchen");
    auto sProv = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    sProv->addDeviceDescription(
        usn,
        UPnPAV::validRendererDeviceDescription(QStringLiteral("Kitchen"), QString{}, QString{}, usn));
    auto* sProvRaw = sProv.get();
    auto rendererFab = std::make_unique<UPnPAV::Doubles::MediaRendererDoubleFactory>();
    auto* rendererFabRaw = rendererFab.get();
    auto prov = RendererProvider{std::make_shared<TestHelper::InMemoryRendererStore>(),
                                 std::move(sProv),
                                 std::move(rendererFab)};
    auto rendererDisconnectSpy = QSignalSpy{&prov, &RendererProvider::rendererDisconnected};
    Q_EMIT sProvRaw->serviceConnected(usn);
    auto const renderer = prov.renderers().at(0);

    Q_EMIT rendererFabRaw->renderer(QStringLiteral("Kitchen"))->unreachable();

    QTRY_COMPARE(renderer->availability(), Renderer::Availability::Offline);
    QCOMPARE(sProvRaw->disconnectedServices(), QStringList{usn});
    QCOMPARE(rendererDisconnectSpy.size(), 1);

    Q_EMIT sProvRaw->serviceConnected(usn);

    QCOMPARE(renderer->availability(), Renderer::Availability::Online);
    QCOMPARE(prov.renderers().size(), 1);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::RendererProviderShould)
