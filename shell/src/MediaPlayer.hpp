// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Renderer.hpp"
#include <QObject>

namespace Shell
{

class MediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Shell::MediaPlayer::PlaybackState playbackState READ playbackState NOTIFY playbackStateChanged)

public:
    /*
     * Inherited QObject constructors
     */
    using QObject::QObject;

    /**
     * The @ref Shell::MediaPlayer::PlaybackState are the states the @ref Shell::MediaPlayer can have.
     */
    enum PlaybackState
    {
        /**
         * The @ref Shell::MediaPlayer is playing something
         */
        Playing,

        /**
         * The @ref Shell::MediaPlayer is paused
         */
        Paused,

        /**
         * The @ref Shell::MediaPlayer is stopped
         */
        Stopped,
    };
    Q_ENUM(PlaybackState)

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
    Q_INVOKABLE void setRenderer(std::shared_ptr<Multimedia::Renderer> const& renderer) noexcept;

    /**
     * Starts the play for the passed @ref Multimedia::Item.
     * The signal @ref playbackStarted is emitted when the playback is successful started.
     */
    Q_INVOKABLE void play(Multimedia::Item const& item);

    /**
     * Gives the current @ref Shell::MediaPlayer::PlaybackState of the @ref Shell::MediaPlayer.
     * The default state is stopped
     * @return The state of the @ref Shell::MediaPlayer.
     */
    PlaybackState playbackState() const noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Shell::MediaPlayer changes it's @ref Shell::MediaPlayer::PlaybackState
     */
    void playbackStateChanged();

private:
    void setPlaybackState(Multimedia::Renderer::State state);

private:
    std::shared_ptr<Multimedia::Renderer> mRenderer = nullptr;
    PlaybackState mState = PlaybackState::Stopped;
};

} // namespace Shell
