// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObject.hpp"

#include <QDebug>
#include <QXmlStreamReader>
#include <utility>

namespace UPnPAV
{

MediaServerObject::MediaServerObject() = default;

MediaServerObject::MediaServerObject(QString id, QString parentId, QString title, QString typeClass)
    : m_id(std::move(id))
    , m_parentId(std::move(parentId))
    , m_title(std::move(title))
    , m_class(std::move(typeClass))
{
}

QString MediaServerObject::id() const noexcept
{
    return m_id;
}

QString MediaServerObject::parentId() const noexcept
{
    return m_parentId;
}

QString MediaServerObject::title() const noexcept
{
    return m_title;
}

QString MediaServerObject::typeClass() const noexcept
{
    return m_class;
}

bool operator==(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept
{
    return ((lhs.m_id == rhs.m_id) && (lhs.m_parentId == rhs.m_parentId) && (lhs.m_title == rhs.m_title) &&
            (lhs.m_class == rhs.m_class));
}

bool operator!=(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept
{
    return !(lhs == rhs);
}

QDebug operator<<(QDebug d, const MediaServerObject &serverObject)
{
    QDebugStateSaver saver(d);

    d.nospace().noquote() << "ID:" << serverObject.id() << "\n";
    d.nospace().noquote() << "Parent ID:" << serverObject.parentId() << "\n";
    d.nospace().noquote() << "Title:" << serverObject.title() << "\n";
    d.nospace().noquote() << "Class:" << serverObject.typeClass() << "\n";

    return d;
}

QVector<MediaServerObject> MediaServerObject::createFromDidl(QString &didl) noexcept
{
    auto didlDesc = didl.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
    QXmlStreamReader didlReader{didlDesc};
    QVector<MediaServerObject> objects;

    while (!didlReader.hasError() && !didlReader.atEnd())
    {
        didlReader.readNextStartElement();
        if (didlReader.isStartElement() &&
            (didlReader.name() == QStringLiteral("container") || didlReader.name() == QStringLiteral("item")))
        {
            auto mediaServerObject = readDidlDesc(didlReader);
            if (mediaServerObject.has_value())
            {
                objects.append(mediaServerObject.value());
            }
        }
    }
    return objects;
}

std::optional<MediaServerObject> MediaServerObject::readDidlDesc(QXmlStreamReader &streamReader) noexcept
{
    QString title;
    QString id;
    QString parentId;
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
            parentId = attribute.value().toString();
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

    if (title.isEmpty() or id.isEmpty() or parentId.isEmpty() or typeClass.isEmpty())
    {
        return std::nullopt;
    }

    auto msObj = MediaServerObject();
    msObj.m_parentId = parentId;
    msObj.m_id = id;
    msObj.m_class = typeClass;
    msObj.m_title = title;

    return msObj;
}

} // namespace UPnPAV
