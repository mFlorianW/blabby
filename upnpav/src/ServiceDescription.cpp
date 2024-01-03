// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceDescription.hpp"
#include <QSharedData>
#include <utility>

namespace UPnPAV
{

ServiceDescription::ServiceDescription() = default;

ServiceDescription::ServiceDescription(QString serviceType,
                                       QString id,
                                       QString scpdUrl,
                                       QString controlUrl,
                                       QString eventUrl)
    : d{new ServiceDescriptionData{std::move(serviceType),
                                   std::move(id),
                                   std::move(scpdUrl),
                                   std::move(controlUrl),
                                   std::move(eventUrl)}}
{
}

QString ServiceDescription::serviceType() const
{
    return d->mServiceType;
}

QString ServiceDescription::id() const
{
    return d->mId;
}

QString ServiceDescription::scpdUrl() const
{
    return d->mScpdUrl;
}

QString ServiceDescription::controlUrl() const
{
    return d->mControlUrl;
}

QString ServiceDescription::eventUrl() const
{
    return d->mEventUrl;
}

bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.d->mServiceType == rhs.d->mServiceType) && (lhs.d->mId == rhs.d->mId) &&
            (lhs.d->mScpdUrl == rhs.d->mScpdUrl) && (lhs.d->mControlUrl == rhs.d->mControlUrl) &&
            (lhs.d->mEventUrl == rhs.d->mEventUrl));
}

bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
