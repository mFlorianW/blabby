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
#ifndef ICONDESCRIPTION_H
#define ICONDESCRIPTION_H

#include "UPnP_Export.h"

#include <QString>

namespace UPnPAV
{

class UPNP_EXPORT IconDescription final
{
public:
    IconDescription();

    IconDescription(const QString &mimeType,
                    quint32 width,
                    quint32 height,
                    quint32 depth,
                    const QString &url);

    const QString &mimeType() const noexcept;

    const QString &url() const noexcept;

    quint32 width() const noexcept;

    quint32 height() const noexcept;

    quint32 depth() const noexcept;

    friend bool operator==(const IconDescription &lhs, const IconDescription &rhs) noexcept;
    friend bool operator!=(const IconDescription &lhs, const IconDescription &rhs) noexcept;

private:
    QString m_mimeType;
    QString m_url;
    QString m_baseUrl;

    quint32 m_width;
    quint32 m_height;
    quint32 m_depth;
};

} // namespace UPnPAV

#endif // ICONDESCRIPTION_H
