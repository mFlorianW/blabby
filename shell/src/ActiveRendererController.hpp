// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererModel.hpp"
#include "Renderer.hpp"
#include <QObject>

namespace Shell
{

/**
 * The @ref Shell::ActiveRendererController follows the Active Renderer of a @ref Shell::MediaRendererModel and gives
 * the Playing screen what it shows of it. Without an Active Renderer it has no name and its Playback State is No Media.
 */
class ActiveRendererController : public QObject
{
    Q_OBJECT

    /**
     * This property is true while there is an Active Renderer.
     */
    Q_PROPERTY(bool hasActiveRenderer READ hasActiveRenderer NOTIFY activeRendererChanged)

    /**
     * This property holds the name of the Active Renderer, empty without an Active Renderer.
     */
    Q_PROPERTY(QString rendererName READ rendererName NOTIFY activeRendererChanged)

    /**
     * This property holds the Playback State of the Active Renderer, No Media without an Active Renderer.
     */
    Q_PROPERTY(Multimedia::Renderer::State playbackState READ playbackState NOTIFY playbackStateChanged)

public:
    /**
     * Creates an instance of the @ref Shell::ActiveRendererController that follows the Active Renderer of the model.
     * @param model The model that holds the Active Renderer, it must outlive the controller.
     */
    explicit ActiveRendererController(MediaRendererModel const& model);

    /**
     * Default destructor
     */
    ~ActiveRendererController() override;

    /*
     * Disabled copy and move semantic.
     */
    Q_DISABLE_COPY_MOVE(ActiveRendererController)

    /**
     * Gives true while there is an Active Renderer.
     */
    bool hasActiveRenderer() const noexcept;

    /**
     * Gives the name of the Active Renderer, empty without an Active Renderer.
     */
    QString rendererName() const noexcept;

    /**
     * Gives the Playback State of the Active Renderer, No Media without an Active Renderer.
     */
    Multimedia::Renderer::State playbackState() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the Active Renderer is changed or cleared.
     */
    void activeRendererChanged();

    /**
     * This signal is emitted when the Playback State changes, also when the Active Renderer is changed or cleared.
     */
    void playbackStateChanged();

    /**
     * This signal is emitted when the Active Renderer went Offline and is no longer the Active Renderer.
     * @param rendererName The name of the Renderer that went Offline.
     */
    void activeRendererWentOffline(QString const& rendererName);

private:
    void onActiveRendererChanged();

    MediaRendererModel const& mModel;
    std::shared_ptr<Multimedia::Renderer> mRenderer = nullptr;
};

} // namespace Shell
