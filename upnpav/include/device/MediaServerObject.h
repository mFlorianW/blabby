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
#ifndef MEDIASERVEROBJECT_H
#define MEDIASERVEROBJECT_H

#include "UPnP_Export.h"

#include <QString>

namespace UPnPAV
{

class UPNP_EXPORT MediaServerObject
{
public:
    MediaServerObject();
    MediaServerObject(const QString &id, const QString &parentId, const QString &title, const QString &typeClass);

    QString id() const noexcept;
    QString parentId() const noexcept;
    QString title() const noexcept;
    QString typeClass() const noexcept;

    friend bool operator==(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;
    friend bool operator!=(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;

private:
    QString m_id;
    QString m_parentId;
    QString m_title;
    QString m_class;
};

UPNP_EXPORT QDebug operator<<(QDebug d, const UPnPAV::MediaServerObject &serverObject);

} // namespace UPnPAV


#endif // MEDIASERVEROBJECT_H
