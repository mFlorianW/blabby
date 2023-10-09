// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BrowseResult.h"
#include "private/LoggingCategories.h"
#include "private/ResponseReader.h"
#include <QXmlStreamReader>

namespace UPnPAV
{

BrowseResult::BrowseResult(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action)
{
    auto reader = ResponseReader{xmlResponse, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::unsignedIntValueRead,
                     &reader,
                     [&](QString const &elementName, quint32 value, ResponseReader::ElementReadResult result) {
                         bool conversionCorrect = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("NumberReturned") and conversionCorrect)
                         {
                             mNumberReturned = value;
                         }
                         else if (elementName == QStringLiteral("TotalMatches") and conversionCorrect)
                         {
                             mTotalMatches = value;
                         }
                         else if (elementName == QStringLiteral("UpdateID") and conversionCorrect)
                         {
                             mUpdateId = value;
                         }
                         else if (result == ResponseReader::ElementReadResult::ConversionError)
                         {
                             qCCritical(upnpavDevice) << "Failed to convert " << elementName;
                         }
                         else if (result == ResponseReader::ElementReadResult::Error)
                         {
                             qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
                         }
                     });

    QObject::connect(
        &reader,
        &ResponseReader::stringValueRead,
        &reader,
        [&](QString const &elementName, QString &value, ResponseReader::ElementReadResult result) {
            if (elementName == QStringLiteral("Result") and result == ResponseReader::ElementReadResult::Ok)
            {
                auto unescapedResult = value.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
                readDidlDescription(unescapedResult);
            }
            else if (result == ResponseReader::ElementReadResult::ConversionError)
            {
                qCCritical(upnpavDevice) << "Failed to convert " << elementName;
            }
            else if (result == ResponseReader::ElementReadResult::Error)
            {
                qCCritical(upnpavDevice) << "Unknown error for value" << elementName;
            }
        });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok)
    {
        qCCritical(upnpavDevice) << "Failed to read GetCurrentConnectionInfo. Response was:" << reader.response();
        return;
    }
}

quint32 BrowseResult::totalMatches() const noexcept
{
    return mTotalMatches;
}

quint32 BrowseResult::numberReturned() const noexcept
{
    return mNumberReturned;
}

quint32 BrowseResult::updateId() const noexcept
{
    return mUpdateId;
}

const QVector<MediaServerObject> &BrowseResult::objects() const noexcept
{
    return m_objects;
}

void BrowseResult::readDidlDescription(const QString &didlDescription)
{
    QXmlStreamReader didlReader{didlDescription};

    while (didlReader.readNext() && !didlReader.hasError() && !didlReader.atEnd())
    {
        if (didlReader.isStartElement() &&
            (didlReader.name() == QStringLiteral("container") || didlReader.name() == QStringLiteral("item")))
        {
            auto mediaServerObject = readDidlObjectDescription(didlReader);
            m_objects.append(mediaServerObject);
        }
    }
}

MediaServerObject BrowseResult::readDidlObjectDescription(QXmlStreamReader &streamReader)
{
    QString title;
    QString id;
    QString parenId;
    QString typeClass;

    // read container attributes
    auto attributes = streamReader.attributes();
    for (const auto &attribute : attributes)
    {
        if (attribute.name() == QStringLiteral("id"))
        {
            id = attribute.value().toString();
        }
        else if (attribute.name() == QStringLiteral("parentID"))
        {
            parenId = attribute.value().toString();
        }
    }

    while (streamReader.readNext() && !streamReader.hasError() && !streamReader.atEnd() &&
           !(streamReader.isEndElement() &&
             ((streamReader.name() == QStringLiteral("container")) || (streamReader.name() == QStringLiteral("item")))))
    {
        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("title"))
        {
            title = streamReader.readElementText();
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("class"))
        {
            typeClass = streamReader.readElementText();
        }
    }

    return MediaServerObject{id, parenId, title, typeClass};
}

} // namespace UPnPAV
