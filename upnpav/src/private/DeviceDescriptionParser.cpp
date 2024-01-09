// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DeviceDescriptionParser.hpp"
#include "ParsingError.hpp"

#include <QDebug>
#include <QUrl>
#include <QVector>

namespace UPnPAV
{

DeviceDescriptionParser::DeviceDescriptionParser(QUrl const& deviceUrl)
    : m_baseUrl(deviceUrl.adjusted(QUrl::RemovePath).toString())
{
}

QVector<DeviceDescription> const& DeviceDescriptionParser::physicalDeviceDescription()
{
    return m_embeddedDevices;
}

void DeviceDescriptionParser::readDeviceDescription(QString const& deviceDescription)
{
    m_streamReader.addData(deviceDescription);

    while (!m_streamReader.atEnd() && !m_streamReader.hasError()) {
        (void)m_streamReader.readNext();
        if ((m_streamReader.isStartDocument()) || (m_streamReader.isEndDocument()) || (m_streamReader.isEndElement())) {
            continue;
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("device"))) {
            m_tempDevices.push_front(readDeviceDescription());
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("URLBase"))) {
            auto baseUrl = m_streamReader.readElementText();
            if (!baseUrl.isEmpty()) {
                m_baseUrl = baseUrl;
            }
        }
    }

    if (m_streamReader.hasError()) {
        qInfo() << "Failed to parse description for:" << m_baseUrl << "Error:" << m_streamReader.errorString();
        throw ParsingError{""};
    }

    createDescriptions();
}

DeviceDescriptionParser::TempDeviceDescription DeviceDescriptionParser::readDeviceDescription()
{
    TempDeviceDescription device;

    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("device"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        (void)m_streamReader.readNext();

        if (m_streamReader.tokenType() == QXmlStreamReader::TokenType::Characters) {
            continue;
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("deviceType"))) {
            device.deviceType = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("friendlyName"))) {
            device.friendlyName = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("manufacturer"))) {
            device.manufacturer = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("modelName"))) {
            device.modelName = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("UDN"))) {
            device.udn = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("iconList"))) {
            device.icons = readIconList();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("deviceList"))) {
            readEmbeddedDevices();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("serviceList"))) {
            device.services = readServiceList();
        }
    }

    return device;
}

void DeviceDescriptionParser::readEmbeddedDevices()
{
    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("deviceList"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        m_streamReader.readNext();

        if (m_streamReader.name().isEmpty()) {
            continue;
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("device"))) {
            m_tempDevices.append(readDeviceDescription());
        }
    }
}

void DeviceDescriptionParser::createDescriptions()
{
    // Build concrete descriptions from the temporay extract descriptions.
    m_embeddedDevices.reserve(m_tempDevices.size());
    for (auto& tempDesc : m_tempDevices) {
        QVector<IconDescription> icons;
        icons.reserve(tempDesc.icons.size());
        for (auto& iconDesc : tempDesc.icons) {
            if (QUrl{iconDesc.url}.isRelative()) {
                iconDesc.url = makeAbsolutePath(m_baseUrl, iconDesc.url);
            }

            icons.append(
                IconDescription{iconDesc.mimeType, iconDesc.width, iconDesc.height, iconDesc.depth, iconDesc.url});
        }

        QVector<ServiceDescription> services;
        services.reserve(tempDesc.services.size());
        for (auto& serviceDesc : tempDesc.services) {
            if (QUrl{serviceDesc.scpdUrl}.isRelative()) {
                serviceDesc.scpdUrl = makeAbsolutePath(m_baseUrl, serviceDesc.scpdUrl);
            }

            if (QUrl{serviceDesc.controlUrl}.isRelative()) {
                serviceDesc.controlUrl = makeAbsolutePath(m_baseUrl, serviceDesc.controlUrl);
            }

            if (QUrl{serviceDesc.eventUrl}.isRelative()) {
                serviceDesc.eventUrl = makeAbsolutePath(m_baseUrl, serviceDesc.eventUrl);
            }

            services.append(ServiceDescription{serviceDesc.serviceType,
                                               serviceDesc.id,
                                               serviceDesc.scpdUrl,
                                               serviceDesc.controlUrl,
                                               serviceDesc.eventUrl});
        }

        m_embeddedDevices.append(DeviceDescription{tempDesc.deviceType,
                                                   tempDesc.friendlyName,
                                                   tempDesc.manufacturer,
                                                   tempDesc.modelName,
                                                   tempDesc.udn,
                                                   icons,
                                                   services});
    }
}

QString DeviceDescriptionParser::makeAbsolutePath(QString const& base, QString const& relativePath)
{
    return relativePath.startsWith("/") ? base + relativePath : base + "/" + relativePath;
}

QVector<DeviceDescriptionParser::TempIconDescription> DeviceDescriptionParser::readIconList()
{
    QVector<TempIconDescription> icons;
    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("iconList"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("icon"))) {
            auto iconDescription = readIconDescription();
            icons.push_back(iconDescription);
        }
    }

    return icons;
}

DeviceDescriptionParser::TempIconDescription DeviceDescriptionParser::readIconDescription()
{
    TempIconDescription icon;

    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("icon"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        m_streamReader.readNext();

        if (m_streamReader.name().isEmpty()) {
            continue;
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("mimetype"))) {
            icon.mimeType = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("width"))) {
            icon.width = m_streamReader.readElementText().toUInt();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("height"))) {
            icon.height = m_streamReader.readElementText().toUInt();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("depth"))) {
            icon.depth = m_streamReader.readElementText().toUInt();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("url"))) {
            icon.url = m_streamReader.readElementText();
        }
    }

    return icon;
}

QVector<DeviceDescriptionParser::TempServiceDescription> DeviceDescriptionParser::readServiceList()
{
    QVector<TempServiceDescription> services;

    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("serviceList"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("service"))) {
            auto serviceDescription = readServiceDescription();
            services.push_back(serviceDescription);
        }
    }

    return services;
}

DeviceDescriptionParser::TempServiceDescription DeviceDescriptionParser::readServiceDescription()
{
    TempServiceDescription service;

    while (!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("service"))) &&
           !m_streamReader.hasError() && !m_streamReader.atEnd()) {
        m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("serviceType"))) {
            service.serviceType = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("serviceId"))) {
            service.id = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("SCPDURL"))) {
            service.scpdUrl = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("controlURL"))) {
            service.controlUrl = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("eventSubURL"))) {
            service.eventUrl = m_streamReader.readElementText();
        }
    }

    return service;
}

} // namespace UPnPAV
