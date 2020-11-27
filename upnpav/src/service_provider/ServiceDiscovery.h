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
#ifndef SERVICEDISCOVERY_H
#define SERVICEDISCOVERY_H

#include <QObject>
#include <QSharedPointer>

class QNetworkDatagram;

namespace UPnPAV
{
class ServiceDiscoveryBackend;

class ServiceDiscovery final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceDiscovery)
public:
    ServiceDiscovery(ServiceDiscoveryBackend *discoveryBackend);
    ~ServiceDiscovery() override;

    void sendSearchRequest(const QString &searchTarget);

Q_SIGNALS:
    void dataReceived(const QNetworkDatagram &response);

private:
    ServiceDiscoveryBackend *m_discoveryBackend;
};

} // namespace UPnPAV
#endif // SERVICEDISCOVERY_H
