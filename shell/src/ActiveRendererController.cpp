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
    }

    mRenderer = activeRenderer;
    if (mRenderer != nullptr) {
        connect(mRenderer.get(), &Renderer::stateChanged, this, &ActiveRendererController::playbackStateChanged);
    }

    Q_EMIT activeRendererChanged();
    Q_EMIT playbackStateChanged();

    if (previous != nullptr and mRenderer == nullptr and previous->availability() == Renderer::Availability::Offline) {
        qCDebug(shell) << "The Active Renderer" << previous->name() << "went Offline.";
        Q_EMIT activeRendererWentOffline(previous->name());
    }
}

} // namespace Shell
