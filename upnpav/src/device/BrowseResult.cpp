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
#include "BrowseResult.h"

#include <QXmlStreamReader>

namespace UPnPAV
{

BrowseResult::BrowseResult(const QString xmlResponse)
{
    QXmlStreamReader responseReader{xmlResponse};
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
            auto unescapedResult = responseReader.readElementText()
                                    .replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
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
    QXmlStreamReader didlReader{didlDescription};

    while(didlReader.readNext() && !didlReader.hasError() && !didlReader.atEnd())
    {
        if(didlReader.isStartElement() && didlReader.name() == "container")
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

    //read container attributes
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
          !(streamReader.isEndElement() && (streamReader.name() == QStringLiteral("container"))) )
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

    return MediaServerObject{id, parenId, title,typeClass};
}

} //namespace UPnPAV
