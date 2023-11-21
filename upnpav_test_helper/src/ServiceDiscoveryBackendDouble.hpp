// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDISCOVERYBACKENDDOUBLE_H
#define SERVICEDISCOVERYBACKENDDOUBLE_H

#include "ServiceDiscoveryBackend.hpp"

#include <QNetworkDatagram>

namespace UPnPAV
{

class ServiceDiscoveryBackendDouble : public ServiceDiscoveryBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceDiscoveryBackendDouble)
public:
    QNetworkDatagram lastSendDatagram;

    ServiceDiscoveryBackendDouble();

    ~ServiceDiscoveryBackendDouble() override;

    void sendResponseForMediaServerRequest(const QNetworkDatagram &datagram);
    void sendNotifyMessage(const QNetworkDatagram &datagram);

protected:
    void sendDiscoveryRequest(const QNetworkDatagram &datagram) override
    {
        lastSendDatagram = datagram;
    }
};

} // namespace UPnPAV

#endif // SERVICEDISCOVERYBACKENDDOUBLE_H
