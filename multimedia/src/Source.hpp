// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Item.hpp"
#include "blabbymultimedia_export.h"
#include <QStack>
#include <QString>
#include <QUrl>

namespace Multimedia
{

/**
 * Definition for a list of items
 */
using Items = QVector<Item>;

/**
 * Forward declaration for pimpl
 */
struct SourcePrivate;

/**
 * Base class for every media source.
 * Each MediaSource shall have a name, optional icon.
 * A MediaSource is a navigatable source of media items.
 * Concret subclasses must implement the @ref navigateTo(QString) function.
 * Subclasses that are not navigatable, all playable items are on the root layer.
 * Those subclasses don't need to overwrite the default implemenation.
 */
class BLABBYMULTIMEDIA_EXPORT Source : public QObject
{
    Q_OBJECT
public:
    /**
     * Default destructor
     */
    ~Source() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(Source)

    /**
     * Gives the name of the source as human readable version.
     * @return The name of the source.
     */
    QString const& sourceName() const noexcept;

    /**
     * Gives the URL for media source icon.
     * This can be a QRC path, FileSystem path or network adress to an Image.
     * If the source doesn't have an image a empty string is shall be returned.
     * @return The URL to the icon, or empty string when the source has no icon.
     */
    QString const& iconUrl() const noexcept;

    /**
     * Gives the current active media items for that media soruce.
     * It's a readonly reference and the MediaSource should be the only writable owner.
     * @return The MediaItemModel for the source.
     */
    Items const& mediaItems() const noexcept;

    /**
     * The source shall navigate to the given path after succesful navigation the @ref mediaItems() must be updated.
     * If it fails to navigate to the given path nothing shall happen.
     * This @ref navigateTo(QString) must only be implemented when the source navigatable.
     * The default implementation does nothing.
     * The source shall emit the @ref navigationFinished(QString) singal when the navigation is succesful finished.
     * @param path The target path to navigate to.
     */
    virtual void navigateTo(QString const& path) noexcept;

    void navigateBack() noexcept;

    void navigateForward() noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the navigation is succesful finished.
     * @param The target path of the navigation.
     */
    void navigationFinished(QString const& path);

protected:
    /**
     * Constructor for subclasses
     */
    Source(QString sourceName, QString iconUrl = QString{""});

    /**
     * The MediaItems of the Source.
     * Subclasses have direct accesses to the variable for easier editing.
     */
    Items mMediaItems;

private:
    std::unique_ptr<SourcePrivate> d;
};

} // namespace Multimedia
