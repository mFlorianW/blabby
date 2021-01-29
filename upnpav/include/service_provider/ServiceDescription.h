// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDESCRIPTION_H
#define SERVICEDESCRIPTION_H

#include "UPnP_Export.h"

#include <QString>

namespace UPnPAV
{

class ServiceDescription
{
public:
    ServiceDescription();

    ServiceDescription(const QString& serviceType,
                       const QString& id,
                       const QString& scpdUrl,
                       const QString& controlUrl,
                       const QString& eventUrl);

    QString serviceType() const;

    QString id() const;

    QString scpdUrl() const;

    QString controlUrl() const;

    QString eventUrl() const;

    friend bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs);
    friend bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs);

private:
    QString m_serviceType;
    QString m_id;
    QString m_scpdUrl;
    QString m_controlUrl;
    QString m_eventUrl;
};

} //namespace UPnPAV

#endif // SERVICEDESCRIPTION_H
