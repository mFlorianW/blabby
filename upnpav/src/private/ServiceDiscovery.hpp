// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDISCOVERY_H
#define SERVICEDISCOVERY_H

#include "blabbyupnpav_export.h"
#include <QObject>
#include <QSharedPointer>

class QNetworkDatagram;

namespace UPnPAV
{
class ServiceDiscoveryBackend;

class BLABBYUPNPAV_EXPORT ServiceDiscovery final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServiceDiscovery)
public:
    ServiceDiscovery(ServiceDiscoveryBackend *discoveryBackend);
    ~ServiceDiscovery() override;

    void sendSearchRequest(QString const &searchTarget);

Q_SIGNALS:
    void dataReceived(QNetworkDatagram const &response);

private:
    ServiceDiscoveryBackend *m_discoveryBackend;
};

} // namespace UPnPAV
#endif // SERVICEDISCOVERY_H
