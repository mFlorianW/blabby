// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDescription.h"

namespace UPnPAV {

ServiceDescription::ServiceDescription()
{
}

ServiceDescription::ServiceDescription(const QString &serviceType,
                                       const QString &id,
                                       const QString &scpdUrl,
                                       const QString &controlUrl,
                                       const QString &eventUrl)
    : m_serviceType(serviceType)
    , m_id(id)
    , m_scpdUrl(scpdUrl)
    , m_controlUrl(controlUrl)
    , m_eventUrl(eventUrl)
{
}

QString ServiceDescription::serviceType() const
{
    return m_serviceType;
}

QString ServiceDescription::id() const
{
    return m_id;
}

QString ServiceDescription::scpdUrl() const
{
    return m_scpdUrl;
}

QString ServiceDescription::controlUrl() const
{
    return m_controlUrl;
}

QString ServiceDescription::eventUrl() const
{
    return m_eventUrl;
}

bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_serviceType == rhs.m_serviceType) &&
            (lhs.m_id == rhs.m_id) &&
            (lhs.m_scpdUrl == rhs.m_scpdUrl) &&
            (lhs.m_controlUrl == rhs.m_controlUrl) &&
            (lhs.m_eventUrl == rhs.m_eventUrl));
}

bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    return !(lhs == rhs);
}

} //namespace UPnPAV
