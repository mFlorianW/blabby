// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ActiveRendererController.hpp"
#include "LoggingCategories.hpp"

using namespace Multimedia;

namespace Shell
{

ActiveRendererController::ActiveRendererController(MediaRendererModel const& model)
    : mModel{model}
{
    connect(&mModel,
            &MediaRendererModel::activeRendererChanged,
            this,
            &ActiveRendererController::onActiveRendererChanged);
    onActiveRendererChanged();
}

ActiveRendererController::~ActiveRendererController() = default;

bool ActiveRendererController::hasActiveRenderer() const noexcept
{
    return mRenderer != nullptr;
}

QString ActiveRendererController::rendererName() const noexcept
{
    if (mRenderer == nullptr) {
        return {};
    }
    return mRenderer->name();
}

Renderer::State ActiveRendererController::playbackState() const noexcept
{
    if (mRenderer == nullptr) {
        return Renderer::State::NoMedia;
    }
    return mRenderer->state();
}

QString ActiveRendererController::trackTitle() const noexcept
{
    return currentTrack().title;
}

QString ActiveRendererController::trackArtist() const noexcept
{
    return currentTrack().artist;
}

QString ActiveRendererController::artworkUrl() const noexcept
{
    return currentTrack().artworkUrl;
}

QString ActiveRendererController::trackAlbum() const noexcept
{
    return currentTrack().album;
}

QString ActiveRendererController::trackYear() const noexcept
{
    return currentTrack().year;
}

QString ActiveRendererController::trackFormat() const noexcept
{
    return currentTrack().format;
}

bool ActiveRendererController::canPause() const noexcept
{
    return mRenderer != nullptr and mRenderer->canPause();
}

bool ActiveRendererController::isTransitioning() const noexcept
{
    return mRenderer != nullptr and mRenderer->isTransitioning();
}

void ActiveRendererController::togglePlayback() noexcept
{
    if (mRenderer == nullptr or mRenderer->isPlaybackControlPending() or mRenderer->isTransitioning()) {
        return;
    }

    if (mRenderer->state() == Renderer::State::Playing) {
        mRenderer->stop();
    } else {
        mRenderer->resume();
    }
}

CurrentTrack ActiveRendererController::currentTrack() const noexcept
{
    if (mRenderer == nullptr) {
        return {};
    }
    return mRenderer->currentTrack();
}

void ActiveRendererController::onActiveRendererChanged()
{
    auto const activeRenderer = mModel.activeRenderer();
    if (activeRenderer == mRenderer) {
        return;
    }

    auto const previous = mRenderer;
    if (previous != nullptr) {
        // disconnect only the own connections, others like the MediaRendererModel keep observing the Renderer.
        disconnect(previous.get(), nullptr, this, nullptr);
        previous->setPositionTracked(false);
    }

    mRenderer = activeRenderer;
    if (mRenderer != nullptr) {
        connect(mRenderer.get(), &Renderer::stateChanged, this, &ActiveRendererController::playbackStateChanged);
        connect(mRenderer.get(), &Renderer::currentTrackChanged, this, &ActiveRendererController::currentTrackChanged);
        connect(mRenderer.get(),
                &Renderer::transitioningChanged,
                this,
                &ActiveRendererController::transitioningChanged);
        connect(mRenderer.get(), &Renderer::controlFailed, this, [this](Renderer::Action action) {
            Q_EMIT controlFailed(mRenderer->name(), action);
        });
        mRenderer->setPositionTracked(true);
    }

    Q_EMIT activeRendererChanged();
    Q_EMIT playbackStateChanged();
    Q_EMIT currentTrackChanged();
    Q_EMIT transitioningChanged();

    if (previous != nullptr and mRenderer == nullptr and previous->availability() == Renderer::Availability::Offline) {
        qCDebug(shell) << "The Active Renderer" << previous->name() << "went Offline.";
        Q_EMIT activeRendererWentOffline(previous->name());
    }
}

} // namespace Shell
