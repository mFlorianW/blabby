// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DescriptionFetcherBackendDouble.hpp"

#include <QFile>

namespace UPnPAV
{
DescriptionFetcherBackendDouble::DescriptionFetcherBackendDouble()
    : DescriptionFetcherBackend()
{
}

DescriptionFetcherBackendDouble::~DescriptionFetcherBackendDouble() = default;

void DescriptionFetcherBackendDouble::fetchDescription(const QUrl &url)
{
    descriptionRequestCalls++;
    lastDescriptionRequest = url;

    // The service provider requests the scpd, so we sent him.
    if (url.toString() == "http://127.0.0.1/test/serviceId1.xml") {
        auto file = QFile{":/xml/ServiceControlPointDefinition.xml"};
        file.open(QIODevice::ReadOnly);
        QString xml = file.readAll();

        Q_EMIT descriptionFetched(xml, QUrl("http://127.0.0.1/test/serviceId1.xml"));
    }
}

void DescriptionFetcherBackendDouble::sendDeviceDescription(const QString xmlFile)
{
    auto file = QFile{xmlFile};
    file.open(QIODevice::ReadOnly);
    QString xml = file.readAll();

    Q_EMIT descriptionFetched(xml, QUrl{"http://127.0.0.1:8000/desc.xml"});
}

void DescriptionFetcherBackendDouble::sendDeviceWithoutServices()
{
    sendDeviceDescription(":/xml/DeviceWithoutEmbeddedDevicesAndServices.xml");
}

void DescriptionFetcherBackendDouble::sendDeviceWithEmbeddedDevicesAndWithoutServices()
{
    sendDeviceDescription(":/xml/DeviceWithEmbeddedDevicesAndWithoutServices.xml");
}

void DescriptionFetcherBackendDouble::sendDeviceDescriptionEmptyBaseUrl()
{
    sendDeviceDescription(":/xml/DeviceEmptyBaseUrl.xml");
}

void DescriptionFetcherBackendDouble::sendDeviceDescriptionBaseUrlAfterDeviceDefinition()
{
    sendDeviceDescription(":/xml/DeviceBaseUrlAfterDeviceDescription.xml");
}

void DescriptionFetcherBackendDouble::sendDeviceWithServicesAndEmbeddedDevicesWithServices()
{
    sendDeviceDescription(":/xml/DeviceWithServicesAndEmbeddedDevicesWithServices.xml");
}

void DescriptionFetcherBackendDouble::sendDeviceXmlNotWellFormed()
{
    sendDeviceDescription(":/xml/DeviceXmlNotWellFormed.xml");
}

void DescriptionFetcherBackendDouble::sendOneDeviceOneService()
{
    sendDeviceDescription(":/xml/OneDeviceOneService.xml");
}

} // namespace UPnPAV
