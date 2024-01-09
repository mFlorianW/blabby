// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Protocol.hpp"
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

/**
 * Declaration for the data of an item to store it with only once.
 */
struct ItemData;

/**
 * Declaration for the item builder to eaiser build the @Multimedia::Item.
 */
class ItemBuilder;

/**
 * An Item is an element of a @ref Multimedia::MediaSource.
 * Basically an Item can have two roles.
 * The first role is the container role and the seconde role is a concrete media item.
 * The container role is used e.g. folder albums etc.
 * Create the @ref Multimedia::Item instance with @ref Multimedia::ItemBuilder.
 */
class BLABBYMULTIMEDIA_EXPORT Item
{
public:
    /**
     * Creates an empty MediaItem instance.
     * This allows to put a MedaItem in a QVariant.
     */
    Item();

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
    QString const& mainText() const noexcept;

    /**
     * Gives the secondary text of the media item.
     * The text depends on the type of the item.
     * E.g. in case of a playable item it can be the artist.
     * @return The main text of the @ref MediaItem.
     */
    QString const& secondaryText() const noexcept;

    /**
     * Give an icon url for that item.
     * The icon is optional and if the item doesn't have an icon an empty QString is returned.
     * @return The url for the icon of the media item.
     */
    QString const& iconUrl() const noexcept;

    /**
     * Gives the path of that item.
     * The format of the path depends on the @ref Multimedia::MediaSource implementation.
     * The path is used for the navigation inside the @ref Multimedia::MediaSource.
     * An empty path is also possible for @ref Multimedia::MediaSource when the source has no navigation.
     * @return The path for the navigation inside the @ref Multimedia::MediaSource.
     */
    QString const& path() const noexcept;

    /**
     * Gives the URL for playing the @ref Multimedia::Item on Renderer.
     * @return A reference to the URL for playing.
     */
    QString const& playUrl() const noexcept;

    /**
     * Gives the supported types of the Item.
     * The supported types are used to check if a @ref Multimedia::Renderer is able to play a @ref Multimedia::Item.
     */
    QVector<UPnPAV::Protocol> const& supportedTypes() const noexcept;

    /**
     * equal operator
     * @param lhs Left hand side of the comparsion
     * @param rhs right hand side of the comparsion
     * @return True equal otherwise false.
     */
    BLABBYMULTIMEDIA_EXPORT friend bool operator==(Item const& lhs, Item const& rhs) noexcept;

    /**
     * unequal operator
     * @param lhs Left hand side of the comparsion
     * @param rhs right hand side of the comparsion
     * @return True not equal otherwise false.
     */
    BLABBYMULTIMEDIA_EXPORT friend bool operator!=(Item const& lhs, Item const& rhs) noexcept;

private:
    friend ItemBuilder;
    QExplicitlySharedDataPointer<ItemData> d;
};

/**
 * Implements the fluent builder pattern for the @ref Multimedia::Item.
 * With this builder it's easier for the @ref Multimedia::Source to create the @ref Multimedia::Item.
 */
class BLABBYMULTIMEDIA_EXPORT ItemBuilder
{
public:
    /**
     * Sets the type of the @ref Multimedia::Item
     * @param type The type of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withItemType(ItemType type) noexcept;

    /**
     * Sets the maint text of the @ref Multimedia::Item
     * @param text The text of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withMainText(QString const& text) noexcept;

    /**
     * Sets the secondary text of the @ref Multimedia::Item
     * @param text The secondary text of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withSecondaryText(QString const& text) noexcept;

    /**
     * Sets the icon URL of the @ref Multimedia::Item
     * @param iconUrl The icon URL of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withIconUrl(QString const& iconUrl) noexcept;

    /**
     * Sets the path of the @ref Multimedia::Item
     * @param path The path of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withPath(QString const& path) noexcept;

    /**
     * Sets the play URL of the @ref Multimedia::Item
     * @param playUrl The playUrl of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withPlayUrl(QString const& playUrl) noexcept;

    /**
     * Sets the supported types of the @ref Multimedia::Item
     * @param supportedTypes The supported types of the @ref Multimedia::Item
     * @return Reference to the @ref Multimedia::ItemBuilder for chaining.
     */
    ItemBuilder& withSupportedTypes(QVector<UPnPAV::Protocol> const& supportedTypes);

    /**
     * Gives the constructed Item.
     * This function should only called when all parameters for the item are set.
     */
    Item build() noexcept;

private:
    Item mItem;
};

struct ItemData : public QSharedData
{
    ItemType mType{ItemType::Container};
    QString mMainText;
    QString mSecondaryText;
    QString mIconUrl;
    QString mPath;
    QString mPlayUrl;
    QVector<UPnPAV::Protocol> mSupportedTypes;

    ItemData() = default;
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
