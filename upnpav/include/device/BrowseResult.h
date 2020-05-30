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
#ifndef BROWSERESULT_H
#define BROWSERESULT_H

#include "UPnP_Export.h"
#include "MediaServerObject.h"

#include <QVector>

class QXmlStreamReader;

namespace UPnPAV
{

class UPNP_EXPORT BrowseResult
{
public:
    BrowseResult(const QString xmlResponse);

    quint32 totalMatches() const noexcept;

    quint32 numberReturned() const noexcept;

    quint32 updateId() const noexcept;

    const QVector<MediaServerObject> &objects() const noexcept;

private:
    void readDidlDescription(const QString &didlDescription);
    MediaServerObject readDidlObjectDescription(QXmlStreamReader &streamReader);

private:
    quint32 m_numberReturned{0};
    quint32 m_totalMatches{0};
    quint32 m_updateId{0};

    QVector<MediaServerObject> m_objects;
};

} //namespace UPnPAV

#endif // BROWSERESULT_H
