// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "Item.hpp"
#include <QUrl>

namespace Multimedia
{

Item::Item()
    : d{new ItemData{}}
{
}

Item::Item(ItemType type, QString mainText, QString secondaryText, QString artworkUrl, QString path)
    : d{new ItemData{type, std::move(mainText), secondaryText, std::move(artworkUrl), std::move(path)}}
{
}

ItemType Item::type() const noexcept
{
    return d->mType;
}

QString const& Item::mainText() const noexcept
{
    return d->mMainText;
}

QString const& Item::secondaryText() const noexcept
{

    return d->mSecondaryText;
}

QString const& Item::artworkUrl() const noexcept
{
    return d->mArtworkUrl;
}

QString const& Item::path() const noexcept
{
    return d->mPath;
}

QString const& Item::playUrl() const noexcept
{
    return d->mPlayUrl;
}

QString const& Item::album() const noexcept
{
    return d->mAlbum;
}

std::optional<std::chrono::milliseconds> Item::duration() const noexcept
{
    return d->mDuration;
}

QVector<UPnPAV::Protocol> const& Item::supportedTypes() const noexcept
{
    return d->mSupportedTypes;
}

bool operator==(Item const& lhs, Item const& rhs) noexcept
{
    // clang-format off
    return (lhs.d == rhs.d) or ((lhs.d->mType == rhs.d->mType) and
                               (lhs.d->mMainText == rhs.d->mMainText) and
                               (lhs.d->mSecondaryText == rhs.d->mSecondaryText) and
                               (lhs.d->mArtworkUrl == rhs.d->mArtworkUrl) and
                               (lhs.d->mPath == rhs.d->mPath) and
                               (lhs.d->mAlbum == rhs.d->mAlbum) and
                               (lhs.d->mDuration == rhs.d->mDuration));
    // clang-format on
}

bool operator!=(Item const& lhs, Item const& rhs) noexcept
{
    return !(lhs == rhs);
}

ItemBuilder& ItemBuilder::withItemType(ItemType type) noexcept
{
    mItem.d->mType = type;
    return *this;
}

ItemBuilder& ItemBuilder::withMainText(QString const& text) noexcept
{
    mItem.d->mMainText = text;
    return *this;
}

ItemBuilder& ItemBuilder::withSecondaryText(QString const& text) noexcept
{
    mItem.d->mSecondaryText = text;
    return *this;
}

ItemBuilder& ItemBuilder::withArtworkUrl(QString const& artworkUrl) noexcept
{
    mItem.d->mArtworkUrl = artworkUrl;
    return *this;
}

ItemBuilder& ItemBuilder::withPath(QString const& path) noexcept
{
    mItem.d->mPath = path;
    return *this;
}

ItemBuilder& ItemBuilder::withPlayUrl(QString const& playUrl) noexcept
{
    mItem.d->mPlayUrl = playUrl;
    return *this;
}

ItemBuilder& ItemBuilder::withAlbum(QString const& album) noexcept
{
    mItem.d->mAlbum = album;
    return *this;
}

ItemBuilder& ItemBuilder::withDuration(std::optional<std::chrono::milliseconds> duration) noexcept
{
    mItem.d->mDuration = duration;
    return *this;
}

ItemBuilder& ItemBuilder::withSupportedTypes(QVector<UPnPAV::Protocol> const& supportedTypes)
{
    mItem.d->mSupportedTypes = supportedTypes;
    return *this;
}

Item ItemBuilder::build() noexcept
{
    return mItem;
}

QString titleOfUri(QString const& uri)
{
    auto title = QUrl{uri}.fileName(QUrl::FullyDecoded);
    auto const extensionStart = title.lastIndexOf(QLatin1Char{'.'});
    if (extensionStart > 0) {
        title.truncate(extensionStart);
    }
    return title;
}

} // namespace Multimedia
