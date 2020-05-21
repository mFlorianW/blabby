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
#ifndef SERVICEDISCOVERYBACKENDDOUBLE_H
#define SERVICEDISCOVERYBACKENDDOUBLE_H

#include "ServiceDiscoveryBackend.h"

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

} //namespace UPnPAV

#endif //SERVICEDISCOVERYBACKENDDOUBLE_H
