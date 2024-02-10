// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaPlayer.hpp"
#include "LoggingCategories.hpp"

using namespace Multimedia;

namespace Shell
{

MediaPlayer::~MediaPlayer() = default;

void MediaPlayer::setRenderer(std::shared_ptr<Multimedia::Renderer> const& renderer) noexcept
{
    // disconnect the old signals before setting a new renderer or clear it.
    if (mRenderer != nullptr) {
        disconnect(mRenderer.get(), nullptr, nullptr, nullptr);
    }

    mRenderer = renderer;
    if (mRenderer != nullptr) {
        connect(mRenderer.get(), &Multimedia::Renderer::stateChanged, this, [this]() {
            setPlaybackState(mRenderer->state());
        });
        mRenderer->initialize();
    }
}

void MediaPlayer::play(Multimedia::Item const& item)
{
    if (mRenderer == nullptr) {
        qCCritical(shell) << "Failed to start playback. No render device set.";
        return;
    }
    mRenderer->playback(item);
}

void MediaPlayer::stop() noexcept
{
    if (mRenderer == nullptr) {
        qCCritical(shell) << "Failed to stop playback. No render device set.";
        return;
    }
    mRenderer->stop();
}

MediaPlayer::PlaybackState MediaPlayer::playbackState() const noexcept
{
    return mState;
}

void MediaPlayer::setPlaybackState(Multimedia::Renderer::State state)
{
    auto pstate = PlaybackState::Stopped;
    if (state == Multimedia::Renderer::State::Playing) {
        pstate = PlaybackState::Playing;
    } else if (state == Multimedia::Renderer::State::Paused) {
        pstate = PlaybackState::Paused;
    }

    if (mState != pstate) {
        mState = pstate;
        Q_EMIT playbackStateChanged();
    }
}

} // namespace Shell
