// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObject.hpp"
#include "private/MediaServerObjectBuilder.hpp"
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

QString MediaServerObject::playUrl() const noexcept
{
    return m_playUrl;
}

bool operator==(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept
{
    return ((lhs.m_id == rhs.m_id) && (lhs.m_parentId == rhs.m_parentId) && (lhs.m_title == rhs.m_title) &&
            (lhs.m_class == rhs.m_class) && (lhs.playUrl() == rhs.playUrl()));
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
    d.nospace().noquote() << "PlayUrl:" << serverObject.playUrl() << "\n";

    return d;
}

QVector<MediaServerObject> MediaServerObject::createFromDidl(QString &didl) noexcept
{
    auto didlDesc = didl.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
    qInfo().noquote() << didl;
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
    MediaServerObjectBuilder builder;
    // read container attributes
    auto attributes = streamReader.attributes();
    for (const auto &attribute : attributes)
    {
        if (attribute.name() == QStringLiteral("id"))
        {
            builder.withId(attribute.value().toString());
        }
        else if (attribute.name() == QStringLiteral("parentID"))
        {
            builder.withParentId(attribute.value().toString());
        }
    }

    while (streamReader.readNext() && !streamReader.hasError() && !streamReader.atEnd() &&
           !(streamReader.isEndElement() &&
             ((streamReader.name() == QStringLiteral("container")) || (streamReader.name() == QStringLiteral("item")))))
    {
        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("title"))
        {
            builder.withTitle(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("class"))
        {
            builder.withTypeClass(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("res"))
        {
            qInfo() << "fdsfsdfsdsdfsdf resss";
            builder.withPlayUrl(streamReader.readElementText());
        }
    }

    if (not builder.isValid())
    {
        return std::nullopt;
    }

    return builder.build();
}

} // namespace UPnPAV
