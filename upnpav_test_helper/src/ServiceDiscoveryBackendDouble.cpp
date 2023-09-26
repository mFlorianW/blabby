// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDiscoveryBackendDouble.h"

namespace UPnPAV
{

ServiceDiscoveryBackendDouble::ServiceDiscoveryBackendDouble()
    : ServiceDiscoveryBackend()
{
}

ServiceDiscoveryBackendDouble::~ServiceDiscoveryBackendDouble()
{
}

void ServiceDiscoveryBackendDouble::sendResponseForMediaServerRequest(const QNetworkDatagram &datagram)
{
    Q_EMIT receivedNetworkDatagram(datagram);
}

void ServiceDiscoveryBackendDouble::sendNotifyMessage(const QNetworkDatagram &datagram)
{
    Q_EMIT receivedNetworkDatagram(datagram);
}

} // namespace UPnPAV
