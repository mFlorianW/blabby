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
#include "DescriptionFetcherBackendDouble.h"

#include <QFile>

namespace UPnPAV
{
DescriptionFetcherBackendDouble::DescriptionFetcherBackendDouble()
    : DescriptionFetcherBackend()
{
}

DescriptionFetcherBackendDouble::~DescriptionFetcherBackendDouble()
{
}

void DescriptionFetcherBackendDouble::fetchDescription(const QUrl &url)
{
    descriptionRequestCalls++;
    lastDescriptionRequest = url;

    //The service provider requests the scpd, so we sent him.
    if(url.toString() == "http://127.0.0.1/test/serviceId1.xml")
    {
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

} //namespace UPnPAV
