// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "RendererProvider.hpp"
#include <QAbstractListModel>

namespace Shell
{

/**
 * This model provides a list based access to all Online @ref Multimedia::Renderer on the network.
 * The rows are ordered alphabetically by the name of the @ref Multimedia::Renderer.
 */
class MediaRendererModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * This property holds the Active Renderer, the @ref Multimedia::Renderer that is used for playback.
     * The Active Renderer is set via the @ref MediaRendererModel::activateRenderer and is nullptr
     * until a @ref Multimedia::Renderer is activated or when the Active Renderer disconnects.
     */
    Q_PROPERTY(std::shared_ptr<Multimedia::Renderer> activeRenderer READ activeRenderer NOTIFY activeRendererChanged)

    /**
     * This property is true while a discovery of @ref Multimedia::Renderer on the network is running.
     * A discovery is started on creation and by @ref MediaRendererModel::rescan and ends when the
     * @ref Multimedia::Renderer on the network had the time to answer it.
     */
    Q_PROPERTY(bool scanning READ isScanning NOTIFY scanningChanged)

public:
    /**
     * DisplayRole definitions for the UI
     */
    enum class DisplayRole
    {
        /**
         * The name of the @ref Multimedia::Renderer.
         */
        Name = Qt::UserRole + 1,
        /**
         * The Playback State of the @ref Multimedia::Renderer as integer value of @ref Multimedia::Renderer::State.
         */
        PlaybackState,
        /**
         * True when the @ref Multimedia::Renderer is the Active Renderer.
         */
        Active
    };
    Q_ENUM(DisplayRole)

    /**
     * Creates an instance of the @ref Shell::MediaRendererModel.
     * @param provider The provider to handle connected and diconnected @ref Multimedia::Renderer.
     */
    MediaRendererModel(std::unique_ptr<Multimedia::RendererProvider> provider);

    /**
     * Default destructor
     */
    ~MediaRendererModel() override;

    /*
     * Disabled copy and move semantic.
     */
    Q_DISABLE_COPY_MOVE(MediaRendererModel)

    /**
     * Gives the number of @ref Multimedia::Renderer elements in the model.
     */
    int rowCount(QModelIndex const& parent = QModelIndex{}) const noexcept override;

    /**
     * Gives the roles to the UI in a form that UI can request the data with @ref MediaRendererModel::data.
     * @return A hash map with the role names and corresponding integer values.
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
     * Gives the active @ref Multimedia::Renderer.
     * This can be nullptr when no active @ref Multimedia::Renderer is set.
     */
    std::shared_ptr<Multimedia::Renderer> activeRenderer() const noexcept;

    /**
     * Makes the @ref Multimedia::Renderer under the passed @ref QModelIndex the Active Renderer.
     * Active means in this case that the renderer is used for playback.
     * The playback of the previous Active Renderer is not touched.
     * If the index is invalid or the @ref Multimedia::Renderer is already active nothing happens.
     */
    Q_INVOKABLE void activateRenderer(QModelIndex const& index);

    /**
     * Gives true while a discovery of @ref Multimedia::Renderer is running.
     */
    bool isScanning() const noexcept;

    /**
     * Starts a new discovery of @ref Multimedia::Renderer on the network.
     * Nothing happens while a discovery is already running.
     */
    Q_INVOKABLE void rescan();

Q_SIGNALS:
    /**
     * This signal is emitted when the Active Renderer is changed or cleared.
     */
    void activeRendererChanged();

    /**
     * This signal is emitted when a discovery starts or ends.
     */
    void scanningChanged();

private Q_SLOTS:
    void onRendererConnected(std::shared_ptr<Multimedia::Renderer> const& renderer);
    void onRendererDisconnected(std::shared_ptr<Multimedia::Renderer> const& renderer);
    void onDiscoveryFinished();

private:
    void onRendererStateChanged(Multimedia::Renderer const* renderer);
    QModelIndex indexOf(Multimedia::Renderer const* renderer) const noexcept;

    std::unique_ptr<Multimedia::RendererProvider> mProvider;
    QVector<std::shared_ptr<Multimedia::Renderer>> mRenderers;
    std::shared_ptr<Multimedia::Renderer> mActiveRenderer = nullptr;
    bool mScanning = false;
};

} // namespace Shell
