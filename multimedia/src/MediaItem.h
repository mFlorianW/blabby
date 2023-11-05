// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "blabbymultimedia_export.h"
#include <QExplicitlySharedDataPointer>
#include <QObject>

namespace Multimedia
{

/**
 * The type of the MediaItem
 */
enum class BLABBYMULTIMEDIA_EXPORT MediaItemType
{
    Container, ///< Container type, e.g. folder on a media server album etc.
    Playable ///< Playable type, a concrete item that can be played on a @ref UPnPAV::MediaRenderer
};

struct MediaItemData;
/**
 * A MediaItem is item of a provider.
 * Basically an MediaItem can have two roles.
 * The first role is the container role and the seconde role is a concrete media item.
 * The container role is used e.g. folder albums etc.
 */
class BLABBYMULTIMEDIA_EXPORT MediaItem
{
public:
    /**
     * Creates an Instance of MediaItem.
     * @param type The Type of the media item.
     * @param mainText The main text of the media item. E.g. song name in of a playable item.
     * @param secondaryText The secondary text of the item. E.g. in case of a playable item the artist.
     * @param iconUrl The URL for icon of the media item.
     */
    MediaItem(MediaItemType type, QString mainText, QString secondaryText = QString(""), QString iconUrl = QString(""));

    /**
     * Gives the type of the @ref MediaItem.
     * @return The type of the @ref MediaItem.
     */
    MediaItemType type() const noexcept;

    /**
     * Gives the main text of the media item.
     * The text depends on the type of the item.
     * E.g. in case of a playable item it can be a song name.
     * @return The main text of the @ref MediaItem.
     */
    QString const &mainText() const noexcept;

    /**
     * Gives the secondary text of the media item.
     * The text depends on the type of the item.
     * E.g. in case of a playable item it can be the artist.
     * @return The main text of the @ref MediaItem.
     */
    QString const &secondaryText() const noexcept;

    /**
     * Give an icon url for that item.
     * The icon is optional and if the item doesn't have an icon an empty QString is returned.
     * @return The url for the icon of the media item.
     */
    QString const &iconUrl() const noexcept;

private:
    QExplicitlySharedDataPointer<MediaItemData> d;
};

struct MediaItemData : public QSharedData
{
    MediaItemType mType{MediaItemType::Container};
    QString mMainText;
    QString mSecondaryText;
    QString mIconUrl;
};

} // namespace Multimedia
