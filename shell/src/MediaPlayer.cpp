// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaPlayer.hpp"
#include "LoggingCategories.hpp"

using namespace Multimedia;

namespace Shell
{

MediaPlayer::~MediaPlayer() = default;

void MediaPlayer::setRenderer(std::shared_ptr<Multimedia::Renderer> const &renderer) noexcept
{
    mRenderer = renderer;
    if (mRenderer != nullptr) {
        connect(mRenderer.get(), &Renderer::playbackStarted, this, &MediaPlayer::playbackStarted);
        mRenderer->initialize();
    }
}

void MediaPlayer::play(Multimedia::Item const &item)
{
    if (mRenderer == nullptr) {
        qCCritical(shell) << "Failed to start playback. No render device set.";
        return;
    }
    mRenderer->playback(item);
}

} // namespace Shell
