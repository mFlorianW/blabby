// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVEROBJECT_H
#define MEDIASERVEROBJECT_H

#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT MediaServerObject
{
public:
    MediaServerObject();
    MediaServerObject(QString id, QString parentId, QString title, QString typeClass);

    QString id() const noexcept;
    QString parentId() const noexcept;
    QString title() const noexcept;
    QString typeClass() const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;
    BLABBYUPNPAV_EXPORT friend bool operator!=(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;

private:
    QString m_id;
    QString m_parentId;
    QString m_title;
    QString m_class;
};

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug d, const UPnPAV::MediaServerObject &serverObject);

} // namespace UPnPAV

#endif // MEDIASERVEROBJECT_H
