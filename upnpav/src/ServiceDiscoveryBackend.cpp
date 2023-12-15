// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDiscoveryBackend.hpp"

#include <QNetworkDatagram>

namespace UPnPAV
{

ServiceDiscoveryBackend::ServiceDiscoveryBackend() = default;
ServiceDiscoveryBackend::~ServiceDiscoveryBackend() = default;

void ServiceDiscoveryBackend::sendSearchRequest(const QNetworkDatagram &requestMessage)
{
    sendDiscoveryRequest(requestMessage);
}

UdpServiceDiscoveryBackend::UdpServiceDiscoveryBackend()
    : ServiceDiscoveryBackend()
{
    // TODO: We need error handling in the constructor. Throw an exception when it fails to initialize the udp socket.
    connect(&m_udpSocket, &QUdpSocket::readyRead, this, &UdpServiceDiscoveryBackend::handleReceivedData);

    if (!m_udpSocket.bind(QHostAddress::AnyIPv4, 1900, QUdpSocket::ShareAddress)) {
        qWarning() << "Failed to bound UPD discovery socket.";
    }

    if (!m_udpSocket.joinMulticastGroup(QHostAddress{"239.255.255.250"})) {
        qWarning() << "Failed to join multicast group.";
        qWarning() << m_udpSocket.errorString();
    }
}

void UdpServiceDiscoveryBackend::sendDiscoveryRequest(const QNetworkDatagram &datagram)
{
    m_udpSocket.writeDatagram(datagram);
}

void UdpServiceDiscoveryBackend::handleReceivedData()
{
    Q_EMIT receivedNetworkDatagram(m_udpSocket.receiveDatagram());
}

} // namespace UPnPAV
