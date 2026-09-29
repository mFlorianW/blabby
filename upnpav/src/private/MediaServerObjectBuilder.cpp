// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObjectBuilder.hpp"

namespace UPnPAV
{

MediaServerObjectBuilder& MediaServerObjectBuilder::withId(QString const& id) noexcept
{
    mObj.mId = id;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withParentId(QString const& parentId) noexcept
{
    mObj.mParentId = parentId;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withTitle(QString const& title) noexcept
{
    mObj.mTitle = title;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withTypeClass(QString const& typeClass) noexcept
{
    mObj.mClass = typeClass;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withPlayUrl(QString const& playUrl) noexcept
{
    mObj.mPlayUrl = playUrl;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withSupportedProtocols(
    QVector<Protocol> const& supportedProtocols) noexcept
{
    mObj.mSupportedProtocols = supportedProtocols;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withAlbumArtUrl(QString const& albumArtUrl) noexcept
{
    mObj.mAlbumArtUrl = albumArtUrl;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withArtist(QString const& artist) noexcept
{
    mObj.mArtist = artist;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withCreator(QString const& creator) noexcept
{
    mObj.mCreator = creator;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withAlbum(QString const& album) noexcept
{
    mObj.mAlbum = album;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withDate(QString const& date) noexcept
{
    mObj.mDate = date;
    return *this;
}

MediaServerObjectBuilder& MediaServerObjectBuilder::withResource(Resource const& resource) noexcept
{
    mObj.mResources.append(resource);
    return *this;
}

bool MediaServerObjectBuilder::isValid() const noexcept
{
    return not mObj.mId.isEmpty() and not mObj.mParentId.isEmpty() and not mObj.mTitle.isEmpty() and
           not mObj.mTitle.isEmpty();
}

MediaServerObject MediaServerObjectBuilder::build()
{
    return mObj;
}

} // namespace UPnPAV
