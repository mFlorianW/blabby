// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObject.h"

#include <QDebug>

namespace UPnPAV
{

MediaServerObject::MediaServerObject() = default;

MediaServerObject::MediaServerObject(const QString &id,
                                     const QString &parentId,
                                     const QString &title,
                                     const QString &typeClass)
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
