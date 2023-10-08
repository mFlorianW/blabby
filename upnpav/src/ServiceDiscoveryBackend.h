// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDISCOVERYBACKEND_H
#define SERVICEDISCOVERYBACKEND_H

#include "blabbyupnpav_export.h"
#include <QObject>
#include <QUdpSocket>

class QNetworkDatagram;

namespace UPnPAV
{

/**
 *
 */
class BLABBYUPNPAV_EXPORT ServiceDiscoveryBackend : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceDiscoveryBackend)
public:
    explicit ServiceDiscoveryBackend();
    virtual ~ServiceDiscoveryBackend();

    void sendSearchRequest(const QNetworkDatagram &requestMessage);

Q_SIGNALS:
    void receivedNetworkDatagram(const QNetworkDatagram &datagram);

protected:
    virtual void sendDiscoveryRequest(const QNetworkDatagram &datagram) = 0;
};

class BLABBYUPNPAV_EXPORT UdpServiceDiscoveryBackend : public ServiceDiscoveryBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(UdpServiceDiscoveryBackend)
public:
    UdpServiceDiscoveryBackend();
    ~UdpServiceDiscoveryBackend() = default;

    void sendDiscoveryRequest(const QNetworkDatagram &datagram) override;

private Q_SLOTS:
    void handleReceivedData();

private:
    QUdpSocket m_udpSocket;
};

} // namespace UPnPAV
#endif // SERVICEDISCOVERYBACKEND_H
