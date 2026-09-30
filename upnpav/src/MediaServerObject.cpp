// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObject.hpp"
#include "private/MediaServerObjectBuilder.hpp"
#include <QDebug>
#include <QRegularExpression>
#include <QXmlStreamReader>
#include <utility>

namespace UPnPAV
{

namespace
{
std::optional<quint32> unsignedAttribute(QXmlStreamAttributes const& attributes, QString const& name)
{
    auto ok = false;
    auto const value = attributes.value(name).toUInt(&ok);
    return ok ? std::optional{value} : std::nullopt;
}

/**
 * Gives the duration of a res element, given as H+:MM:SS[.F+] or H+:MM:SS[.F0/F1], e.g. 0:04:31.250.
 */
std::optional<std::chrono::milliseconds> durationAttribute(QXmlStreamAttributes const& attributes)
{
    static auto const durationExpression =
        QRegularExpression{QStringLiteral(R"(^(\d+):(\d{2}):(\d{2})(?:\.(\d+)(?:/(\d+))?)?$)")};
    auto const match = durationExpression.matchView(attributes.value(QStringLiteral("duration")));
    if (not match.hasMatch()) {
        return std::nullopt;
    }

    auto const hours = std::chrono::hours{match.captured(1).toLongLong()};
    auto const minutes = std::chrono::minutes{match.captured(2).toLongLong()};
    auto const seconds = std::chrono::seconds{match.captured(3).toLongLong()};
    auto fraction = std::chrono::milliseconds{0};
    if (match.hasCaptured(5)) {
        auto const denominator = match.captured(5).toLongLong();
        if (denominator == 0) {
            return std::nullopt;
        }
        fraction = std::chrono::milliseconds{match.captured(4).toLongLong() * 1000 / denominator};
    } else if (match.hasCaptured(4)) {
        // Only the milliseconds of a decimal fraction are kept, e.g. .25 is 250 ms.
        fraction = std::chrono::milliseconds{match.captured(4).left(3).leftJustified(3, QLatin1Char{'0'}).toLongLong()};
    }
    return hours + minutes + seconds + fraction;
}
} // namespace

MediaServerObject::MediaServerObject() = default;

MediaServerObject::MediaServerObject(QString id, QString parentId, QString title, QString typeClass)
    : mId(std::move(id))
    , mParentId(std::move(parentId))
    , mTitle(std::move(title))
    , mClass(std::move(typeClass))
{
}

QString MediaServerObject::id() const noexcept
{
    return mId;
}

QString MediaServerObject::parentId() const noexcept
{
    return mParentId;
}

QString MediaServerObject::title() const noexcept
{
    return mTitle;
}

QString MediaServerObject::typeClass() const noexcept
{
    return mClass;
}

QString MediaServerObject::playUrl() const noexcept
{
    return mPlayUrl;
}

QString MediaServerObject::albumArtUrl() const noexcept
{
    return mAlbumArtUrl;
}

QString MediaServerObject::artist() const noexcept
{
    return mArtist;
}

QString MediaServerObject::creator() const noexcept
{
    return mCreator;
}

QString MediaServerObject::album() const noexcept
{
    return mAlbum;
}

QString MediaServerObject::date() const noexcept
{
    return mDate;
}

QVector<Resource> const& MediaServerObject::resources() const noexcept
{
    return mResources;
}

QVector<Protocol> MediaServerObject::supportedProtocols() const noexcept
{
    return mSupportedProtocols;
}

bool operator==(MediaServerObject const& lhs, MediaServerObject const& rhs) noexcept
{
    return ((lhs.mId == rhs.mId) and (lhs.mParentId == rhs.mParentId) and (lhs.mTitle == rhs.mTitle) and
            (lhs.mClass == rhs.mClass) and (lhs.playUrl() == rhs.playUrl()) and
            (lhs.supportedProtocols() == rhs.supportedProtocols()) and (lhs.mAlbumArtUrl == rhs.mAlbumArtUrl) and
            (lhs.mArtist == rhs.mArtist) and (lhs.mCreator == rhs.mCreator) and (lhs.mAlbum == rhs.mAlbum) and
            (lhs.mDate == rhs.mDate) and (lhs.mResources == rhs.mResources));
}

bool operator!=(MediaServerObject const& lhs, MediaServerObject const& rhs) noexcept
{
    return !(lhs == rhs);
}

QDebug operator<<(QDebug d, MediaServerObject const& serverObject)
{
    QDebugStateSaver saver(d);

    d.nospace().noquote() << "ID:" << serverObject.id() << "\n";
    d.nospace().noquote() << "Parent ID:" << serverObject.parentId() << "\n";
    d.nospace().noquote() << "Title:" << serverObject.title() << "\n";
    d.nospace().noquote() << "Class:" << serverObject.typeClass() << "\n";
    d.nospace().noquote() << "PlayUrl:" << serverObject.playUrl() << "\n";
    d.nospace().noquote() << "SupportedProtocols:" << serverObject.supportedProtocols() << "\n";
    d.nospace().noquote() << "AlbumArtUrl:" << serverObject.albumArtUrl() << "\n";
    d.nospace().noquote() << "Artist:" << serverObject.artist() << "\n";
    d.nospace().noquote() << "Creator:" << serverObject.creator() << "\n";
    d.nospace().noquote() << "Album:" << serverObject.album() << "\n";
    d.nospace().noquote() << "Date:" << serverObject.date() << "\n";
    for (auto const& resource : serverObject.resources()) {
        d.nospace().noquote() << "Resource:" << resource.uri << " " << resource.protocolInfo << "\n";
    }

    return d;
}

QVector<MediaServerObject> MediaServerObject::createFromDidl(QString& didl) noexcept
{
    auto didlDesc = didl.replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
    QXmlStreamReader didlReader{didlDesc};
    QVector<MediaServerObject> objects;

    while (!didlReader.hasError() && !didlReader.atEnd()) {
        didlReader.readNextStartElement();
        if (didlReader.isStartElement() &&
            (didlReader.name() == QStringLiteral("container") || didlReader.name() == QStringLiteral("item"))) {
            auto mediaServerObject = readDidlDesc(didlReader);
            if (mediaServerObject.has_value()) {
                objects.append(mediaServerObject.value());
            }
        }
    }
    return objects;
}

std::optional<MediaServerObject> MediaServerObject::readDidlDesc(QXmlStreamReader& streamReader) noexcept
{
    MediaServerObjectBuilder builder;
    // An artist in a role, e.g. the AlbumArtist, is only taken when there is no artist without a role.
    auto artistInRole = QString{};
    auto hasArtist = false;
    // Servers may list the album art in several sizes, the first one is usually the thumbnail.
    auto hasAlbumArt = false;
    // read container attributes
    auto const attributes = streamReader.attributes();
    for (auto const& attribute : attributes) {
        if (attribute.name() == QStringLiteral("id")) {
            builder.withId(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("parentID")) {
            builder.withParentId(attribute.value().toString());
        }
    }

    while (streamReader.readNext() && !streamReader.hasError() && !streamReader.atEnd() &&
           !(streamReader.isEndElement() && ((streamReader.name() == QStringLiteral("container")) ||
                                             (streamReader.name() == QStringLiteral("item"))))) {
        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("title")) {
            builder.withTitle(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("class")) {
            builder.withTypeClass(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("albumArtURI") && not hasAlbumArt) {
            builder.withAlbumArtUrl(streamReader.readElementText());
            hasAlbumArt = true;
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("creator")) {
            builder.withCreator(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("artist")) {
            auto const hasRole = streamReader.attributes().hasAttribute(QStringLiteral("role"));
            auto const artist = streamReader.readElementText();
            if (not hasRole and not hasArtist) {
                builder.withArtist(artist);
                hasArtist = true;
            } else if (hasRole and artistInRole.isEmpty()) {
                artistInRole = artist;
            }
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("album")) {
            builder.withAlbum(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("date")) {
            builder.withDate(streamReader.readElementText());
        }

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("res")) {
            auto const attributes = streamReader.attributes();
            auto resource =
                Resource{.uri = QString{},
                         .protocolInfo = attributes.value(QStringLiteral("protocolInfo")).toString(),
                         .bitsPerSample = unsignedAttribute(attributes, QStringLiteral("bitsPerSample")),
                         .sampleFrequency = unsignedAttribute(attributes, QStringLiteral("sampleFrequency")),
                         .duration = durationAttribute(attributes)};
            for (auto const& attribute : attributes) {
                if (attribute.name() == QStringLiteral("protocolInfo")) {
                    auto protos = QVector<Protocol>{};
                    auto const rawProtos = attribute.value().toString().split(";");
                    for (auto const& rawProto : rawProtos) {
                        auto const proto = Protocol::create(rawProto);
                        if (proto.has_value()) {
                            protos.push_back(std::move(proto.value()));
                        }
                    }
                    builder.withSupportedProtocols(protos);
                }
            }
            resource.uri = streamReader.readElementText();
            builder.withPlayUrl(resource.uri);
            builder.withResource(resource);
        }
    }
    // The object is only complete once all its elements are read, the title needn't be the first one.
    if (not builder.isValid()) {
        return std::nullopt;
    }
    if (not hasArtist) {
        builder.withArtist(artistInRole);
    }
    return builder.build();
}

} // namespace UPnPAV
