// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICEDESCRIPTION_H
#define SERVICEDESCRIPTION_H

#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>
#include <QString>

namespace UPnPAV
{

struct ServiceDescriptionData;
class BLABBYUPNPAV_EXPORT ServiceDescription
{
public:
    ServiceDescription();

    ServiceDescription(QString serviceType, QString id, QString scpdUrl, QString controlUrl, QString eventUrl);

    QString serviceType() const;

    QString id() const;

    QString scpdUrl() const;

    QString controlUrl() const;

    QString eventUrl() const;

    BLABBYUPNPAV_EXPORT friend bool operator==(const ServiceDescription &lhs, const ServiceDescription &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(const ServiceDescription &lhs, const ServiceDescription &rhs);

private:
    QExplicitlySharedDataPointer<ServiceDescriptionData> d;
};

struct ServiceDescriptionData : public QSharedData
{
    QString mServiceType;
    QString mId;
    QString mScpdUrl;
    QString mControlUrl;
    QString mEventUrl;

    ServiceDescriptionData(QString serviceType, QString id, QString scpdUrl, QString controlUrl, QString eventUrl)
        : QSharedData()
        , mServiceType{std::move(serviceType)}
        , mId{std::move(id)}
        , mScpdUrl{std::move(scpdUrl)}
        , mControlUrl{std::move(controlUrl)}
        , mEventUrl{std::move(eventUrl)}
    {
    }

    ~ServiceDescriptionData() = default;

    ServiceDescriptionData(ServiceDescriptionData const &other) noexcept = default;
    ServiceDescriptionData &operator=(ServiceDescriptionData const &other) noexcept = delete;

    ServiceDescriptionData(ServiceDescriptionData &&other) noexcept = default;
    ServiceDescriptionData &operator=(ServiceDescriptionData &&other) noexcept = delete;
};

} // namespace UPnPAV

Q_DECLARE_TYPEINFO(UPnPAV::ServiceDescription, Q_RELOCATABLE_TYPE);

#endif // SERVICEDESCRIPTION_H
