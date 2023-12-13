// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVEROBJECT_H
#define MEDIASERVEROBJECT_H

#include "blabbyupnpav_export.h"
#include <QString>
#include <QXmlStreamReader>

namespace UPnPAV
{
class MediaServerObjectBuilder;
/**
 * A MediaServerObject is an item provided by @ref UPnPAV::MediaServer.
 */
class BLABBYUPNPAV_EXPORT MediaServerObject
{
public:
    MediaServerObject();
    MediaServerObject(QString id, QString parentId, QString title, QString typeClass);

    QString id() const noexcept;
    QString parentId() const noexcept;
    QString title() const noexcept;
    QString typeClass() const noexcept;

    /**
     * Gives the URL for playing the underlying object in @ref UPnPAV::MediaRenderer
     * return The URL for playing the underlying object.
     */
    QString playUrl() const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;
    BLABBYUPNPAV_EXPORT friend bool operator!=(const MediaServerObject &lhs, const MediaServerObject &rhs) noexcept;

    /**
     * Factory method that creates all @ref UPnPAV::MediaServerObject from a DIDL.
     * The result contains only @ref UPnPAV::MediaServerObject where all required parameters could be parsed.
     * @param didl The raw/unescaped DIDL string.
     * @return A list with all parsed @ref UPnPAV::MediaServerObject
     */
    static QVector<MediaServerObject> createFromDidl(QString &didl) noexcept;

private:
    friend UPnPAV::MediaServerObjectBuilder;
    static std::optional<MediaServerObject> readDidlDesc(QXmlStreamReader &streamReader) noexcept;
    QString m_id;
    QString m_parentId;
    QString m_title;
    QString m_class;
    QString m_playUrl;
};

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug d, const UPnPAV::MediaServerObject &serverObject);

} // namespace UPnPAV

#endif // MEDIASERVEROBJECT_H
