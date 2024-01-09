// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "RendererProvider.hpp"
#include <QAbstractListModel>

namespace Shell
{

/**
 * This model provides a list based access to all discovered UPnPAV MediaRenderers on the network.
 */
class MediaRendererModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * This property holds the active @ref Multimedia::Renderer for playback.
     * The active @ref Multimedia::Renderer is set via the @ref MediaRendererModel::activateRenderer
     */
    Q_PROPERTY(std::shared_ptr<Multimedia::Renderer> activeRenderer READ activeRenderer NOTIFY activeRendererChanged)

public:
    /**
     * DisplayRole definitions for the UI
     */
    enum class DisplayRole
    {
        MediaRendererTitle = Qt::UserRole + 1,
        MediaRendererIconUrl,
        MediaRendererActive
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
     * Marks the @ref Multimedia::Renderer under the passed @ref QModelIndex as active.
     * Active means in this case that the renderer is used for playback.
     * If the index is invalid or not @ref Multimedia::Renderer was found for that index nothing happens.
     */
    Q_INVOKABLE void activateRenderer(QModelIndex const& index);

Q_SIGNALS:
    /**
     * This signal is emitted when the active @ref Multimedia::Renderer is changed with @ref
     * Multimedia::activeRendererChanged.
     */
    void activeRendererChanged();

private Q_SLOTS:
    void onRendererConnected(std::shared_ptr<Multimedia::Renderer> const& renderer);
    void onRendererDisconnected(std::shared_ptr<Multimedia::Renderer> const& renderer);

private:
    std::unique_ptr<Multimedia::RendererProvider> mProvider;
    QVector<std::shared_ptr<Multimedia::Renderer>> mRenderers;
    std::shared_ptr<Multimedia::Renderer> mActiveRenderer = nullptr;
};

} // namespace Shell
