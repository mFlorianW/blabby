// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Item.hpp"
#include "blabbymultimedia_export.h"
#include <QStack>
#include <QString>
#include <QUrl>
#include <memory>

namespace Multimedia
{

/**
 * Definition for a list of items
 */
using Items = QVector<Item>;

/**
 * A page of the Items of a Container, requested by @ref Multimedia::Source::browsePage independent of the navigation
 * of the Source. It finishes once, with the Items or failed, see @ref finished().
 * A page can outlive the Source that gave it, it may still finish then.
 */
class BLABBYMULTIMEDIA_EXPORT PendingPage : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates an unfinished page.
     */
    PendingPage();

    /**
     * Default destructor
     */
    ~PendingPage() override;

    /**
     * Disable copy and move
     */
    Q_DISABLE_COPY_MOVE(PendingPage)

    /**
     * Gives whether the page finished, with its Items or failed.
     * @return True when the page finished.
     */
    bool isFinished() const noexcept;

    /**
     * Gives whether loading the page failed.
     * @return True when the page finished and loading it failed.
     */
    bool hasFailed() const noexcept;

    /**
     * Gives the Items of the page.
     * @return The Items of the page, empty until it finished.
     */
    Items const& items() const noexcept;

    /**
     * Gives the total number of Items in the Container, as reported with the page.
     * @return The total number of Items, 0 until the page finished.
     */
    qsizetype totalItemCount() const noexcept;

    /**
     * Finishes the page with its Items, called by the Source. Does nothing when the page already finished.
     * @param items The Items of the page.
     * @param totalItemCount The total number of Items in the Container.
     */
    void finish(Items items, qsizetype totalItemCount) noexcept;

    /**
     * Finishes the page as failed, called by the Source. Does nothing when the page already finished.
     */
    void fail() noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the page finished, with its Items or failed.
     */
    void finished();

private:
    Items mItems;
    qsizetype mTotalItemCount{0};
    bool mFinished{false};
    bool mFailed{false};
};

/**
 * Forward declaration for pimpl
 */
struct SourcePrivate;

/**
 * Base class for every media source.
 * Each MediaSource shall have a name, optional icon.
 * A MediaSource is a navigatable source of media items.
 * Concrete subclasses must implement the @ref navigate(QString, qsizetype) function.
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
     * Gives the total number of Items in the current Container, as last reported by the Source.
     * It is never less than the number of loaded @ref mediaItems().
     */
    qsizetype totalItemCount() const noexcept;

    /**
     * Gives true while the current Container holds Items that aren't loaded yet, see @ref loadMore().
     */
    bool canLoadMore() const noexcept;

    /**
     * Navigates to the given path, see @ref navigate(QString, qsizetype).
     * @param path The target path to navigate to.
     * @param minimumItemCount How many Items shall be loaded at least, a paged Source loads the first page otherwise.
     */
    void navigateTo(QString const& path, qsizetype minimumItemCount = 0) noexcept;

    /**
     * Loads the next page of Items of the current Container and appends them to the @ref mediaItems().
     * This @ref loadMore() must only be implemented when the Source is paged, the default implementation does nothing.
     * The Source shall emit the @ref moreItemsLoaded() signal when the page is loaded. If it fails to load the page
     * the Source shall emit the @ref loadingMoreFailed() signal instead, and keep its @ref mediaItems() unchanged.
     */
    virtual void loadMore() noexcept;

    /**
     * Navigates back to the previous path in the navigation history.
     * @param minimumItemCount How many Items shall be loaded at least, a paged Source loads the first page otherwise.
     */
    void navigateBack(qsizetype minimumItemCount = 0) noexcept;

    void navigateForward() noexcept;

    /**
     * Requests the page of the Items of the Container at the path that starts at the index, independent of the
     * navigation: the @ref mediaItems() and the current Container stay. The Source decides how many Items a page holds,
     * a page without Items ends the Container.
     * A paged Source must implement it, the default implementation gives a finished page without Items.
     * The page may already be finished when it's given.
     * @param path The path of the Container.
     * @param startIndex The index of the first Item of the page.
     * @return The page, it finishes later or is finished already.
     */
    virtual std::unique_ptr<PendingPage> browsePage(QString const& path, qsizetype startIndex) noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the navigation is succesful finished.
     * @param The target path of the navigation.
     */
    void navigationFinished(QString const& path);

    /**
     * This signal is emitted when the navigation failed, the @ref mediaItems() are unchanged.
     * @param path The target path of the navigation.
     */
    void navigationFailed(QString const& path);

    /**
     * This signal is emitted when the next page of the current Container is loaded and its Items are appended.
     */
    void moreItemsLoaded();

    /**
     * This signal is emitted when loading the next page of the current Container failed, the @ref mediaItems() are
     * unchanged.
     */
    void loadingMoreFailed();

protected:
    /**
     * Constructor for subclasses
     */
    Source(QString sourceName, QString iconUrl = QString{""});

    /**
     * Sets the total number of Items in the current Container, as reported by a paged Source.
     */
    void setTotalItemCount(qsizetype count) noexcept;

    /**
     * The source shall navigate to the given path, after a successful navigation the @ref mediaItems() must be updated.
     * A paged Source loads the first page of the Container, or more pages until at least the minimum number of Items
     * is loaded or the Container has no more Items, see @ref loadMore().
     * This @ref navigate(QString, qsizetype) must only be implemented when the source is navigable.
     * The default implementation does nothing.
     * The source shall emit the @ref navigationFinished(QString) signal when the navigation finished successfully.
     * If it fails to navigate to the given path the source shall emit the @ref navigationFailed(QString) signal
     * instead, and keep its @ref mediaItems() unchanged.
     * @param path The target path to navigate to.
     * @param minimumItemCount How many Items shall be loaded at least.
     */
    virtual void navigate(QString const& path, qsizetype minimumItemCount) noexcept;

    /**
     * The MediaItems of the Source.
     * Subclasses have direct accesses to the variable for easier editing.
     */
    Items mMediaItems;

private:
    std::unique_ptr<SourcePrivate> d;
};

} // namespace Multimedia
