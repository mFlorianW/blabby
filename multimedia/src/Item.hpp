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
enum class BLABBYMULTIMEDIA_EXPORT ItemType
{
    Container, ///< Container type, e.g. folder on a media server album etc.
    Playable ///< Playable type, a concrete item that can be played on a @ref UPnPAV::MediaRenderer
};

struct ItemData;
/**
 * An Item is an element of a @ref Multimedia::MediaSource.
 * Basically an Item can have two roles.
 * The first role is the container role and the seconde role is a concrete media item.
 * The container role is used e.g. folder albums etc.
 */
class BLABBYMULTIMEDIA_EXPORT Item
{
public:
    /**
     * Creates an empty MediaItem instance.
     * This allows to put a MedaItem in a QVariant.
     */
    Item() = default;

    /**
     * Creates an Instance of MediaItem.
     * @param type The Type of the media item.
     * @param mainText The main text of the media item. E.g. song name in of a playable item.
     * @param secondaryText The secondary text of the item. E.g. in case of a playable item the artist.
     * @param iconUrl The URL for icon of the media item.
     * @param path The path of the media item, the path is used for the navigation in the source.
     */
    Item(ItemType type,
         QString mainText,
         QString secondaryText = QString(""),
         QString iconUrl = QString(""),
         QString path = QString(""));

    /**
     * Gives the type of the @ref MediaItem.
     * @return The type of the @ref MediaItem.
     */
    ItemType type() const noexcept;

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

    /**
     * Gives the path of that item.
     * The format of the path depends on the @ref Multimedia::MediaSource implementation.
     * The path is used for the navigation inside the @ref Multimedia::MediaSource.
     * An empty path is also possible for @ref Multimedia::MediaSource when the source has no navigation.
     * @return The path for the navigation inside the @ref Multimedia::MediaSource.
     */
    QString const &path() const noexcept;

    /**
     * equal operator
     * @param lhs Left hand side of the comparsion
     * @param rhs right hand side of the comparsion
     * @return True equal otherwise false.
     */
    BLABBYMULTIMEDIA_EXPORT friend bool operator==(Item const &lhs, Item const &rhs) noexcept;

    /**
     * unequal operator
     * @param lhs Left hand side of the comparsion
     * @param rhs right hand side of the comparsion
     * @return True not equal otherwise false.
     */
    BLABBYMULTIMEDIA_EXPORT friend bool operator!=(Item const &lhs, Item const &rhs) noexcept;

private:
    QExplicitlySharedDataPointer<ItemData> d;
};

struct ItemData : public QSharedData
{
    ItemType mType{ItemType::Container};
    QString mMainText;
    QString mSecondaryText;
    QString mIconUrl;
    QString mPath;

    ItemData(ItemType type,
             QString mainText,
             QString secondaryText = QString(""),
             QString iconUrl = QString(""),
             QString path = QString(""))
        : QSharedData{}
        , mType{type}
        , mMainText{std::move(mainText)}
        , mSecondaryText{std::move(secondaryText)}
        , mIconUrl{std::move(iconUrl)}
        , mPath{std::move(path)}
    {
    }
};

} // namespace Multimedia
