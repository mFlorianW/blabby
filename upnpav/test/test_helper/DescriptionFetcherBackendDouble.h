// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DESCRIPTIONFETCHERBACKENDDOUBLE_H
#define DESCRIPTIONFETCHERBACKENDDOUBLE_H

#include "DescriptionFetcherBackend.h"

#include <QUrl>

namespace UPnPAV
{

class DescriptionFetcherBackendDouble : public DescriptionFetcherBackend
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DescriptionFetcherBackendDouble)
public:
    DescriptionFetcherBackendDouble();

    ~DescriptionFetcherBackendDouble() override;

    void sendDeviceDescription(const QString xmlFile);

    void sendDeviceWithoutServices();

    void sendDeviceWithEmbeddedDevicesAndWithoutServices();

    void sendDeviceDescriptionEmptyBaseUrl();

    void sendDeviceDescriptionBaseUrlAfterDeviceDefinition();

    void sendDeviceWithServicesAndEmbeddedDevicesWithServices();

    void sendDeviceXmlNotWellFormed();

    void sendOneDeviceOneService();

    QUrl lastDescriptionRequest;
    quint32 descriptionRequestCalls{0};
protected:
    void fetchDescription(const QUrl &url) override;
};

} //namespace UPnPAV

#endif //DESCRIPTIONFETCHERBACKENDDOUBLE_H
