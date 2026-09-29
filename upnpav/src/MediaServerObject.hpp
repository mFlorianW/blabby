// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVEROBJECT_H
#define MEDIASERVEROBJECT_H

#include "Protocol.hpp"
#include "blabbyupnpav_export.h"
#include <QString>
#include <QXmlStreamReader>
#include <optional>

namespace UPnPAV
{
class MediaServerObjectBuilder;

/**
 * A resource of a @ref UPnPAV::MediaServerObject, e.g. one encoding of a track.
 */
struct BLABBYUPNPAV_EXPORT Resource
{
    /**
     * The URI of the resource.
     */
    QString uri;

    /**
     * The protocol info of the resource, e.g. http-get:*:audio/flac:*.
     */
    QString protocolInfo;

    /**
     * The bits per sample of the resource, unset when unknown.
     */
    std::optional<quint32> bitsPerSample;

    /**
     * The sample frequency of the resource in Hz, unset when unknown.
     */
    std::optional<quint32> sampleFrequency;

    friend bool operator==(Resource const& lhs, Resource const& rhs) = default;
};

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
    QVector<Protocol> supportedProtocols() const noexcept;

    /**
     * Gives the URL for playing the underlying object in @ref UPnPAV::MediaRenderer
     * return The URL for playing the underlying object.
     */
    QString playUrl() const noexcept;

    /**
     * Gives the URL of the album art of the object (upnp:albumArtURI), the first one when there are several.
     * @return The album art URL or an empty string when the object has none.
     */
    QString albumArtUrl() const noexcept;

    /**
     * Gives the artist of the object (upnp:artist). An artist without a role is preferred over one with a role,
     * e.g. the AlbumArtist.
     * @return The artist or an empty string when the object has none.
     */
    QString artist() const noexcept;

    /**
     * Gives the creator of the object (dc:creator).
     * @return The creator or an empty string when the object has none.
     */
    QString creator() const noexcept;

    /**
     * @return The album of the object, empty when unknown.
     */
    QString album() const noexcept;

    /**
     * @return The date of the object as given by the server, e.g. 2024-03-01, empty when unknown.
     */
    QString date() const noexcept;

    /**
     * @return The resources of the object in the order of the DIDL.
     */
    QVector<Resource> const& resources() const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(MediaServerObject const& lhs, MediaServerObject const& rhs) noexcept;
    BLABBYUPNPAV_EXPORT friend bool operator!=(MediaServerObject const& lhs, MediaServerObject const& rhs) noexcept;

    /**
     * Factory method that creates all @ref UPnPAV::MediaServerObject from a DIDL.
     * The result contains only @ref UPnPAV::MediaServerObject where all required parameters could be parsed.
     * @param didl The raw/unescaped DIDL string.
     * @return A list with all parsed @ref UPnPAV::MediaServerObject
     */
    static QVector<MediaServerObject> createFromDidl(QString& didl) noexcept;

private:
    friend UPnPAV::MediaServerObjectBuilder;
    static std::optional<MediaServerObject> readDidlDesc(QXmlStreamReader& streamReader) noexcept;
    QString mId;
    QString mParentId;
    QString mTitle;
    QString mClass;
    QString mPlayUrl;
    QString mAlbumArtUrl;
    QString mArtist;
    QString mCreator;
    QString mAlbum;
    QString mDate;
    QVector<Resource> mResources;
    QVector<Protocol> mSupportedProtocols;
};

BLABBYUPNPAV_EXPORT QDebug operator<<(QDebug d, UPnPAV::MediaServerObject const& serverObject);

} // namespace UPnPAV

#endif // MEDIASERVEROBJECT_H
