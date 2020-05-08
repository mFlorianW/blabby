/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef SERVICEDISCOVERYBACKEND_H
#define SERVICEDISCOVERYBACKEND_H

#include "UPnP_Export.h"

#include <QObject>
#include <QUdpSocket>

class QNetworkDatagram;

namespace UPnPAV
{

/**
 *
 */
class UPNP_EXPORT ServiceDiscoveryBackend : public QObject
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

class UPNP_EXPORT UdpServiceDiscoveryBackend : public ServiceDiscoveryBackend
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

} //namespace UPnPAV
#endif // SERVICEDISCOVERYBACKEND_H
