// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObject.hpp"
#include "private/MediaServerObjectBuilder.hpp"
#include <QDebug>
#include <QXmlStreamReader>
#include <utility>

namespace UPnPAV
{

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

QVector<Protocol> MediaServerObject::supportedProtocols() const noexcept
{
    return mSupportedProtocols;
}

bool operator==(MediaServerObject const &lhs, MediaServerObject const &rhs) noexcept
{
    return ((lhs.mId == rhs.mId) and (lhs.mParentId == rhs.mParentId) and (lhs.mTitle == rhs.mTitle) and
            (lhs.mClass == rhs.mClass) and (lhs.playUrl() == rhs.playUrl()) and
            (lhs.supportedProtocols() == rhs.supportedProtocols()));
}

bool operator!=(MediaServerObject const &lhs, MediaServerObject const &rhs) noexcept
{
    return !(lhs == rhs);
}

QDebug operator<<(QDebug d, MediaServerObject const &serverObject)
{
    QDebugStateSaver saver(d);

    d.nospace().noquote() << "ID:" << serverObject.id() << "\n";
    d.nospace().noquote() << "Parent ID:" << serverObject.parentId() << "\n";
    d.nospace().noquote() << "Title:" << serverObject.title() << "\n";
    d.nospace().noquote() << "Class:" << serverObject.typeClass() << "\n";
    d.nospace().noquote() << "PlayUrl:" << serverObject.playUrl() << "\n";
    d.nospace().noquote() << "SupportedProtocols:" << serverObject.supportedProtocols() << "\n";

    return d;
}

QVector<MediaServerObject> MediaServerObject::createFromDidl(QString &didl) noexcept
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

std::optional<MediaServerObject> MediaServerObject::readDidlDesc(QXmlStreamReader &streamReader) noexcept
{
    MediaServerObjectBuilder builder;
    // read container attributes
    auto attributes = streamReader.attributes();
    for (auto const &attribute : attributes) {
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

        if (streamReader.isStartElement() && streamReader.name() == QStringLiteral("res")) {
            auto const attributes = streamReader.attributes();
            for (auto const &attribute : attributes) {
                if (attribute.name() == QStringLiteral("protocolInfo")) {
                    auto protos = QVector<Protocol>{};
                    for (auto const &rawProto : attribute.value().toString().split(";")) {
                        auto const proto = Protocol::create(rawProto);
                        if (proto.has_value()) {
                            protos.push_back(std::move(proto.value()));
                        }
                    }
                    builder.withSupportedProtocols(protos);
                }
            }
            builder.withPlayUrl(streamReader.readElementText());
        }

        if (not builder.isValid()) {
            return std::nullopt;
        }
    }
    return builder.build();
}

} // namespace UPnPAV
