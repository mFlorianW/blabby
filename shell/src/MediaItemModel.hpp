// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include <QAbstractListModel>

namespace Shell
{
/**
 * The MediaItemModel provides access to the item of a media source and make it possible to navigate them.
 * The Items of a large Container are fetched page by page through Qt's fetch more, see @ref canFetchMore().
 */
class MediaItemModel : public QAbstractListModel
{
    Q_OBJECT
    /**
     * This property holds the name of the active @ref Multimedia::MediaSource
     */
    Q_PROPERTY(QString mediaSourceName READ mediaSourceName NOTIFY mediaSourceChanged)

    /**
     * This property holds the icon URL of the active @ref Multimedia::MediaSource
     */
    Q_PROPERTY(QString mediaSourceIconUrl READ mediaSourceIconUrl NOTIFY mediaSourceChanged)

    /**
     * This property is true while a @ref Multimedia::MediaSource is set, i.e. there is an Active Source.
     */
    Q_PROPERTY(bool hasMediaSource READ hasMediaSource NOTIFY mediaSourceChanged)

    /**
     * This property is true while a Container is opening, until its Items arrive or opening it fails.
     * Activations and navigating back are ignored meanwhile.
     */
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

    /**
     * This property holds the title of the current Container, empty at the root Container.
     */
    Q_PROPERTY(QString containerTitle READ containerTitle NOTIFY containerChanged)

    /**
     * This property is true while the current Container is the root Container of the Active Source.
     */
    Q_PROPERTY(bool atRoot READ isAtRoot NOTIFY containerChanged)

    /**
     * This property is true when fetching more Items of the current Container failed, the fetched Items stay.
     * No more Items are fetched until @ref retryLoadMore() is called or another Container is opened.
     */
    Q_PROPERTY(bool loadMoreFailed READ hasLoadMoreFailed NOTIFY loadMoreFailedChanged)
public:
    enum class DisplayRole
    {
        MediaItemTitle = Qt::UserRole + 1,
        /**
         * The URL of the artwork of the @ref Multimedia::Item, e.g. the album art, empty when it has none.
         */
        MediaItemArtworkUrl,
        /**
         * The type of the @ref Multimedia::Item as integer value of @ref Multimedia::ItemType.
         */
        MediaItemType,
        /**
         * The secondary text of the @ref Multimedia::Item, e.g. the artist, empty when it has none.
         */
        MediaItemSecondaryText,
    };
    Q_ENUM(DisplayRole)

    /**
     * Inherited base class constructors
     */
    using QAbstractListModel::QAbstractListModel;

    /**
     * Default destructor
     */
    ~MediaItemModel() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(MediaItemModel)

    /**
     * Gives the amout of media items in the model.
     */
    int rowCount(QModelIndex const& index) const noexcept override;

    /**
     * Gives the roles names for the UI to access the for displaying in the model.
     * @return A QHash with all the role names and identifier.
     */
    QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * Gives the requested data for the UI.
     * The data are requested by the index, index range is from 0..@ref rowCount(QModelIndex) -1
     * Which type of value is requeted is defined by the role parameter.
     * @param index The item index for which the values are requested.
     * @param role The data that shall be returned for item referred by the index.
     * @return A QVariant with the stored data or empty QVariant for an invalid index or role parameter.
     */
    QVariant data(QModelIndex const& index, int role) const noexcept override;

    /**
     * Gives true when the current Container holds Items that aren't fetched yet and can be fetched now.
     * It is false while a Container opens, while more Items are fetched and after fetching more failed.
     * @param parent Only the invalid root index has Items.
     */
    bool canFetchMore(QModelIndex const& parent) const noexcept override;

    /**
     * Fetches the next page of Items of the current Container, when @ref canFetchMore() allows it.
     * The Items are appended when they arrive, when that fails @ref hasLoadMoreFailed() becomes true.
     * @param parent Only the invalid root index has Items.
     */
    void fetchMore(QModelIndex const& parent) noexcept override;

    /**
     * Sets the media source for the model.
     * The model only contains the @ref Multimedia::MediaItem from the passed source.
     * To clear the model pass a nullptr to the function.
     * @param mediaSrc The model from that the @ref Multimedia::MediaItem are retrived.
     */
    Q_INVOKABLE void setMediaSource(std::shared_ptr<Multimedia::Source> const& mediaSrc);

    /**
     * Activates the @ref Multimedia::MediaItem under the passed index.
     * Activating a Container opens it. Activating a Playable requests to play it, see @ref playRequested. Any
     * activation while @ref isBusy() does nothing.
     * Opening a Container remembers how many Items the current Container has loaded and its scroll position, both are
     * restored when navigating back to it.
     * @param idx The index of the @ref Multimedia::MediaItem that shall be activated.
     * @param scrollPosition The index of the first Item the view shows of the current Container.
     */
    Q_INVOKABLE void activateMediaItem(qsizetype idx, qsizetype scrollPosition = 0) noexcept;

    /**
     * Navigates the @ref Shell::MediaItemModel back to the parent Container.
     * The parent Container is loaded again up to the number of Items it had loaded, then its scroll position is
     * restored, see @ref scrollPositionRestoreRequested().
     * Does nothing at the root Container or while @ref isBusy().
     */
    Q_INVOKABLE void navigateBack() noexcept;

    /**
     * Fetches the next page of Items again after fetching it failed. Does nothing when it didn't fail.
     */
    Q_INVOKABLE void retryLoadMore() noexcept;

    /**
     * Gives the name of the active @ref Multimedia::MediaSource.
     * Is no @ref Multimedia::MediaSource active an empty name will be returned.
     * @return The name of the active @ref Multimedia::MediaSource
     */
    QString mediaSourceName() const noexcept;

    /**
     * Gives the icon URL of the active @ref Multimedia::MediaSource
     * Is no @ref Multimedia::MediaSource active an empty icon URL will be returned.
     * @return The icon URL of the active @ref Multimedia::MediaSource
     */
    QString mediaSourceIconUrl() const noexcept;

    /**
     * Gives true while a @ref Multimedia::MediaSource is set.
     */
    bool hasMediaSource() const noexcept;

    /**
     * Gives true while a Container is opening.
     */
    bool isBusy() const noexcept;

    /**
     * Gives the title of the current Container, an empty string at the root Container.
     */
    QString containerTitle() const noexcept;

    /**
     * Gives true while the current Container is the root Container.
     */
    bool isAtRoot() const noexcept;

    /**
     * Gives true when fetching more Items of the current Container failed.
     */
    bool hasLoadMoreFailed() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Multimedia::MediaSource in the model is changed.
     */
    void mediaSourceChanged();

    /**
     * This signal is emitted when the busy state changes.
     */
    void busyChanged();

    /**
     * This signal is emitted when the current Container changes to another level, i.e. its title or the root flag.
     */
    void containerChanged();

    /**
     * This signal is emitted when fetching more Items fails, or the failure is cleared.
     */
    void loadMoreFailedChanged();

    /**
     * This signal is emitted when opening a Container, or returning to the parent Container, failed.
     * The model stays on the current Container with its Items.
     * @param containerTitle The title of the Container that couldn't be opened, the name of the
     *                       @ref Multimedia::MediaSource for its root Container.
     */
    void containerOpenFailed(QString const& containerTitle);

    /**
     * This signal is emitted when a Playable is activated, to play it through the Queue.
     * @param playable The activated Playable.
     */
    void playRequested(Multimedia::Item const& playable);

    /**
     * This signal is emitted after navigating back, when the Items of the parent Container are shown again.
     * The view shall scroll back to the position it had when the Container was opened.
     * @param scrollPosition The index of the first Item to show, clamped to the last Item when the Container shrank.
     */
    void scrollPositionRestoreRequested(qsizetype scrollPosition);

private:
    /**
     * The navigation that the model requested and whose Items haven't arrived yet.
     */
    enum class PendingNavigation
    {
        None,
        Open,
        Back,
    };

    /**
     * A Container opened from the root, with what is needed to return to its parent Container.
     */
    struct OpenedContainer
    {
        QString mTitle;
        qsizetype mParentItemCount{0};
        qsizetype mParentScrollPosition{0};
    };

    void startNavigation(PendingNavigation navigation, OpenedContainer container) noexcept;
    void onNavigationFinished() noexcept;
    void onNavigationFailed() noexcept;
    void onMoreItemsLoaded() noexcept;
    void onLoadingMoreFailed() noexcept;
    void setLoadMoreFailed(bool failed) noexcept;
    QString parentContainerTitle() const noexcept;

    std::shared_ptr<Multimedia::Source> mMediaSrc;
    // The Containers opened from the root, the last one is the current Container.
    QList<OpenedContainer> mOpenedContainers;
    PendingNavigation mPendingNavigation{PendingNavigation::None};
    // The Container that opens, or the parent Container while navigating back.
    OpenedContainer mPendingContainer;
    // The rows the model reports, the Source appends fetched Items before the model inserts their rows.
    int mRowCount{0};
    bool mLoadingMore{false};
    bool mLoadMoreFailed{false};
};

} // namespace Shell
