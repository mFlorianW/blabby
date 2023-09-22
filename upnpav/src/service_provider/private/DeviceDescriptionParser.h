// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef DESCRIPTIONREADER_H
#define DESCRIPTIONREADER_H

#include "DeviceDescription.h"
#include "blabbyupnpav_export.h"
#include <QString>
#include <QXmlStreamReader>

class QUrl;

namespace UPnPAV
{
class IconDescription;

class BLABBYUPNPAV_EXPORT DeviceDescriptionParser
{
public:
    DeviceDescriptionParser(const QUrl &deviceUrl);

    const QVector<DeviceDescription> &physicalDeviceDescription();

    void readDeviceDescription(const QString &deviceDescription);

private:
    struct TempIconDescription
    {
        QString mimeType;
        quint32 width;
        quint32 height;
        quint32 depth;
        QString url;
    };

    struct TempServiceDescription
    {
        QString serviceType;
        QString id;
        QString scpdUrl;
        QString controlUrl;
        QString eventUrl;
    };

    struct TempDeviceDescription
    {
        QString deviceType;
        QString friendlyName;
        QString manufacturer;
        QString modelName;
        QString udn;

        QVector<DeviceDescriptionParser::TempIconDescription> icons;
        QVector<DeviceDescriptionParser::TempServiceDescription> services;
    };

private:
    TempDeviceDescription readDeviceDescription();

    QVector<TempIconDescription> readIconList();

    TempIconDescription readIconDescription();

    QVector<TempServiceDescription> readServiceList();
    TempServiceDescription readServiceDescription();

    void readEmbeddedDevices();

    void createDescriptions();

    static QString makeAbsolutePath(const QString &base, const QString &relativePath);

private:
    QVector<TempDeviceDescription> m_tempDevices;

    QVector<DeviceDescription> m_embeddedDevices;

    QString m_baseUrl;

    QXmlStreamReader m_streamReader;
};

} // namespace UPnPAV

#endif // DESCRIPTIONREADER_H
