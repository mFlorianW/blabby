// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
