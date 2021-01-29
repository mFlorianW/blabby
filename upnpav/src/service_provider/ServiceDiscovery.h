// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
