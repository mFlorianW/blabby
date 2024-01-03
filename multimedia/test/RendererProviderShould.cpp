// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RendererProviderShould.hpp"
#include "Descriptions.hpp"
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
    auto prov = RendererProvider{std::move(sProv)};

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
    auto prov = RendererProvider{std::move(sProv)};
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
    auto prov = RendererProvider{std::move(sProv)};
    auto rendererDisconnectSpy = QSignalSpy{&prov, &RendererProvider::rendererDisconnected};

    Q_EMIT sProvRaw->serviceConnected(usn);
    Q_EMIT sProvRaw->serviceDisconnected(usn);
    QCOMPARE(rendererDisconnectSpy.size(), 1);
    QCOMPARE_NE(rendererDisconnectSpy.at(0).at(0).value<std::shared_ptr<Renderer>>(), nullptr);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::RendererProviderShould)
