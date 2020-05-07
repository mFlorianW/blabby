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
#ifndef DESCRIPTIONREADER_H
#define DESCRIPTIONREADER_H

#include "DeviceDescription.h"

#include <QString>
#include <QXmlStreamReader>

class QUrl;

namespace UPnPAV
{
class IconDescription;

class DeviceDescriptionParser
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

} //namespace DescriptionReader;

#endif // DESCRIPTIONREADER_H
