// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "Renderer.hpp"
#include <QObject>

namespace Shell
{

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    /*
     * Inherited QObject constructors
     */
    using QObject::QObject;

    /**
     * Default destructor
     */
    ~MediaPlayer() override;

    /*
     * Disable copay and move semantic
     */
    Q_DISABLE_COPY_MOVE(MediaPlayer)

    /**
     * Sets the renderer for @ref Shell::MediaPlayer that is used for the playback.
     * @param renderer The renderer for controlling the playback.
     */
    Q_INVOKABLE void setRenderer(std::shared_ptr<Multimedia::Renderer> const &renderer) noexcept;

    /**
     * Starts the play for the passed @ref Multimedia::Item.
     * The signal @ref playbackStarted is emitted when the playback is successful started.
     */
    Q_INVOKABLE void play(Multimedia::Item const &item);

Q_SIGNALS:
    /**
     * This signal is emitted when the playback started
     */
    void playbackStarted();

private:
    std::shared_ptr<Multimedia::Renderer> mRenderer = nullptr;
};

} // namespace Shell
