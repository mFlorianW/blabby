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
#include "MediaServerObject.h"

#include <QDebug>

namespace UPnPAV
{

MediaServerObject::MediaServerObject() = default;

MediaServerObject::MediaServerObject(const QString &id, const QString &parentId, const QString &title, const QString &typeClass)
    : m_id(id)
    , m_parentId(parentId)
    , m_title(title)
    , m_class(typeClass)
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

} // namespace UPnPAV
