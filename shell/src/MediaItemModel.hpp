// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
public:
    enum class DisplayRole
    {
        MediaItemTitle = Qt::UserRole + 1,
        MediaItemIconUrl
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
     * Sets the media source for the model.
     * The model only contains the @ref Multimedia::MediaItem from the passed source.
     * To clear the model pass a nullptr to the function.
     * @param mediaSrc The model from that the @ref Multimedia::MediaItem are retrived.
     */
    Q_INVOKABLE void setMediaSource(std::shared_ptr<Multimedia::Source> const& mediaSrc);

    /**
     * Activates the @ref Multimedia::MediaItem under the passed index.
     * If the @ref Multimedia::MediaItem is a container item then the container will be opened.
     * For a playable item the signal @ref playRequest will be emitted.
     * @param idx The index of the @ref Multimedia::MediaItem that shall be activated.
     */
    Q_INVOKABLE void activateMediaItem(qsizetype idx) noexcept;

    /**
     * Navigates the @ref Shell::MediaItemModel to the previous layer if possible.
     */
    Q_INVOKABLE void navigateBack() const noexcept;

    /**
     * Navigates the @ref Shell::MediaItemModel to the last visited layer if possible.
     */
    Q_INVOKABLE void navigateForward() const noexcept;

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

Q_SIGNALS:
    /**
     * This signal is emitted when a playabale MediaItem is activated.
     */
    void playRequest(Multimedia::Item const& mediaItem);

    /**
     * This signal is emitted when the @ref Multimedia::MediaSource in the model is changed.
     */
    void mediaSourceChanged();

private:
    std::shared_ptr<Multimedia::Source> mMediaSrc;
};

} // namespace Shell
