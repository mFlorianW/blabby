// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDiscovery.h"
#include "ServiceDiscoveryBackend.h"

#include <QNetworkDatagram>

namespace UPnPAV
{

namespace
{
constexpr char MULTICAST_ADDRESS[]{ "239.255.255.250" };
constexpr quint16 MULTICAST_PORT{ 1900 };
} // namespace

ServiceDiscovery::ServiceDiscovery(ServiceDiscoveryBackend *discoveryBackend)
    : QObject()
    , m_discoveryBackend(discoveryBackend)
{
    (void)connect(m_discoveryBackend, &ServiceDiscoveryBackend::receivedNetworkDatagram, this, &ServiceDiscovery::dataReceived);
}

ServiceDiscovery::~ServiceDiscovery()
{
}

void ServiceDiscovery::sendSearchRequest(const QString &searchTarget)
{
    auto searchPayload = QString{ "M-SEARCH * HTTP/1.1\r\n"
                                  "Host: 239.255.255.250:1900\r\n"
                                  "Man: \"ssdp:discover\"\r\n"
                                  "MX: 3\r\n"
                                  "ST: %1\r\n"
                                  "USER-AGENT: Linux/1.0 UPnP/1.0 test/0.1.0"
                                  "\r\n" }
                             .arg(searchTarget);

    QNetworkDatagram searchDatagram{ searchPayload.toUtf8() };
    searchDatagram.setDestination(QHostAddress{ MULTICAST_ADDRESS }, MULTICAST_PORT);

    m_discoveryBackend->sendSearchRequest(searchDatagram);
}

} // namespace UPnPAV
