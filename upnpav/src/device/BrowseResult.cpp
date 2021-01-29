// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BrowseResult.h"

#include <QXmlStreamReader>

namespace UPnPAV
{

BrowseResult::BrowseResult(const QString xmlResponse)
{
    QXmlStreamReader responseReader{ xmlResponse };
    while(responseReader.readNext() && !responseReader.atEnd() && !responseReader.hasError())
    {
        if(responseReader.isStartElement() && responseReader.name() == "NumberReturned")
        {
            m_numberReturned = responseReader.readElementText().toUInt();
        }
        else if(responseReader.isStartElement() && responseReader.name() == "TotalMatches")
        {
            m_totalMatches = responseReader.readElementText().toUInt();
        }
        else if(responseReader.isStartElement() && responseReader.name() == "UpdateID")
        {
            m_updateId = responseReader.readElementText().toUInt();
        }
        else if(responseReader.isStartElement() && responseReader.name() == "Result")
        {
            auto unescapedResult =
                responseReader.readElementText().replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
            readDidlDescription(unescapedResult);
        }
    }
}

quint32 BrowseResult::totalMatches() const noexcept
{
    return m_totalMatches;
}

quint32 BrowseResult::numberReturned() const noexcept
{
    return m_numberReturned;
}

quint32 BrowseResult::updateId() const noexcept
{
    return m_updateId;
}

const QVector<MediaServerObject> &BrowseResult::objects() const noexcept
{
    return m_objects;
}

void BrowseResult::readDidlDescription(const QString &didlDescription)
{
    QXmlStreamReader didlReader{ didlDescription };

    while(didlReader.readNext() && !didlReader.hasError() && !didlReader.atEnd())
    {
        if(didlReader.isStartElement() && (didlReader.name() == "container" || didlReader.name() == "item"))
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
    for(const auto &attribute : attributes)
    {
        if(attribute.name() == "id")
        {
            id = attribute.value().toString();
        }
        else if(attribute.name() == "parentID")
        {
            parenId = attribute.value().toString();
        }
    }

    while(streamReader.readNext() && !streamReader.hasError() && !streamReader.atEnd() &&
          !(streamReader.isEndElement() && ((streamReader.name() == "container") || (streamReader.name() == "item"))))
    {
        if(streamReader.isStartElement() && streamReader.name() == "title")
        {
            title = streamReader.readElementText();
        }

        if(streamReader.isStartElement() && streamReader.name() == "class")
        {
            typeClass = streamReader.readElementText();
        }
    }

    return MediaServerObject{ id, parenId, title, typeClass };
}

} // namespace UPnPAV
