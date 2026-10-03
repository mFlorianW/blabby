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

qint64 ActiveRendererController::position() const noexcept
{
    return mRenderer != nullptr ? mRenderer->position().count() : 0;
}

bool ActiveRendererController::hasDuration() const noexcept
{
    return mRenderer != nullptr and mRenderer->duration().has_value();
}

qint64 ActiveRendererController::duration() const noexcept
{
    return mRenderer != nullptr ? mRenderer->duration().value_or(std::chrono::milliseconds{0}).count() : 0;
}

bool ActiveRendererController::canSeek() const noexcept
{
    return mRenderer != nullptr and mRenderer->canSeek();
}

void ActiveRendererController::seek(qint64 position) noexcept
{
    if (mRenderer != nullptr) {
        mRenderer->seek(std::chrono::milliseconds{position});
    }
}

int ActiveRendererController::volume() const noexcept
{
    return mRenderer != nullptr ? static_cast<int>(mRenderer->volume()) : 0;
}

int ActiveRendererController::volumeMinimum() const noexcept
{
    return mRenderer != nullptr ? static_cast<int>(mRenderer->volumeMinimum()) : 0;
}

int ActiveRendererController::volumeMaximum() const noexcept
{
    return mRenderer != nullptr ? static_cast<int>(mRenderer->volumeMaximum()) : 100;
}

bool ActiveRendererController::canControlVolume() const noexcept
{
    return mRenderer != nullptr and mRenderer->canControlVolume();
}

void ActiveRendererController::setVolume(int volume) noexcept
{
    if (mRenderer != nullptr and volume >= 0) {
        mRenderer->setVolume(static_cast<quint32>(volume));
    }
}

bool ActiveRendererController::isMuted() const noexcept
{
    return mRenderer != nullptr and mRenderer->isMuted();
}

bool ActiveRendererController::canControlMute() const noexcept
{
    return mRenderer != nullptr and mRenderer->canControlMute();
}

void ActiveRendererController::setMuted(bool muted) noexcept
{
    if (mRenderer != nullptr) {
        mRenderer->setMuted(muted);
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
        connect(mRenderer.get(), &Renderer::positionChanged, this, &ActiveRendererController::positionChanged);
        connect(mRenderer.get(), &Renderer::volumeChanged, this, &ActiveRendererController::volumeChanged);
        connect(mRenderer.get(), &Renderer::muteChanged, this, &ActiveRendererController::muteChanged);
        connect(mRenderer.get(), &Renderer::durationChanged, this, &ActiveRendererController::durationChanged);
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
    Q_EMIT positionChanged();
    Q_EMIT durationChanged();
    Q_EMIT volumeChanged();
    Q_EMIT muteChanged();

    if (previous != nullptr and mRenderer == nullptr and previous->availability() == Renderer::Availability::Offline) {
        qCDebug(shell) << "The Active Renderer" << previous->name() << "went Offline.";
        Q_EMIT activeRendererWentOffline(previous->name());
    }
}

} // namespace Shell
