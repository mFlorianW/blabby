// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRendererModel.hpp"
#include "LoggingCategories.hpp"

using namespace Multimedia;

namespace Shell
{

MediaRendererModel::MediaRendererModel(std::unique_ptr<RendererProvider> provider)
    : mProvider{std::move(provider)}
{
    Q_ASSERT(mProvider != nullptr);

    connect(mProvider.get(), &RendererProvider::rendererConnected, this, &MediaRendererModel::onRendererConnected);
    connect(mProvider.get(),
            &RendererProvider::rendererDisconnected,
            this,
            &MediaRendererModel::onRendererDisconnected);
    connect(mProvider.get(), &RendererProvider::discoveryFinished, this, &MediaRendererModel::onDiscoveryFinished);

    mScanning = true;
    mProvider->discover();
}

MediaRendererModel::~MediaRendererModel() = default;

int MediaRendererModel::rowCount(QModelIndex const& parent) const noexcept
{
    Q_UNUSED(parent);
    return static_cast<int>(mRenderers.size());
}

QHash<int, QByteArray> MediaRendererModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(DisplayRole::Name), QByteArray{"name"}),
        std::make_pair(static_cast<int>(DisplayRole::PlaybackState), QByteArray{"playbackState"}),
        std::make_pair(static_cast<int>(DisplayRole::Active), QByteArray{"active"}),
        std::make_pair(static_cast<int>(DisplayRole::Manufacturer), QByteArray{"manufacturer"}),
        std::make_pair(static_cast<int>(DisplayRole::ModelName), QByteArray{"modelName"}),
        std::make_pair(static_cast<int>(DisplayRole::Address), QByteArray{"address"}),
    };
    return roles;
}

QVariant MediaRendererModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or (index.row() >= mRenderers.size())) {
        qCritical(shell) << "Failed to request data. Error: invalid index:" << index.row()
                         << "Renderers size:" << mRenderers.size();
        return {};
    }

    auto const renderer = mRenderers.at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::Name) {
        return renderer->name();
    } else if (dispRole == DisplayRole::PlaybackState) {
        return static_cast<int>(renderer->state());
    } else if (dispRole == DisplayRole::Active) {
        return renderer == mActiveRenderer;
    } else if (dispRole == DisplayRole::Manufacturer) {
        return renderer->manufacturer();
    } else if (dispRole == DisplayRole::ModelName) {
        return renderer->modelName();
    } else if (dispRole == DisplayRole::Address) {
        return renderer->address();
    }
    return {};
}

std::shared_ptr<Multimedia::Renderer> MediaRendererModel::activeRenderer() const noexcept
{
    return mActiveRenderer;
}

void MediaRendererModel::activateRenderer(QModelIndex const& index)
{
    if (not index.isValid() or index.row() >= mRenderers.size()) {
        qCritical(shell) << "Failed to set active Renderer. Error: invalid index:" << index.row()
                         << "Renderers size:" << mRenderers.size();
        return;
    }

    auto const renderer = mRenderers.at(index.row());
    if (mActiveRenderer == renderer) {
        return;
    }

    auto const previousIndex = indexOf(mActiveRenderer.get());
    mActiveRenderer = renderer;
    qCDebug(shell) << "Activate renderer for index" << index.row() << ".";
    Q_EMIT activeRendererChanged();
    if (previousIndex.isValid()) {
        Q_EMIT dataChanged(previousIndex, previousIndex, {static_cast<int>(DisplayRole::Active)});
    }
    Q_EMIT dataChanged(index, index, {static_cast<int>(DisplayRole::Active)});
}

bool MediaRendererModel::isScanning() const noexcept
{
    return mScanning;
}

void MediaRendererModel::rescan()
{
    if (mScanning) {
        return;
    }

    qCDebug(shell) << "Rescan the network for Renderers.";
    mScanning = true;
    Q_EMIT scanningChanged();
    mProvider->discover();
}

void MediaRendererModel::onRendererConnected(std::shared_ptr<Renderer> const& renderer)
{
    // upper_bound keeps Renderers with equal names in the order they appeared.
    auto const pos = std::upper_bound(mRenderers.cbegin(),
                                      mRenderers.cend(),
                                      renderer,
                                      [](std::shared_ptr<Renderer> const& lhs, std::shared_ptr<Renderer> const& rhs) {
                                          return lhs->name().compare(rhs->name(), Qt::CaseInsensitive) < 0;
                                      });
    auto const newIndex = static_cast<int>(std::distance(mRenderers.cbegin(), pos));
    beginInsertRows(QModelIndex{}, newIndex, newIndex);
    mRenderers.insert(newIndex, renderer);
    endInsertRows();

    connect(renderer.get(), &Renderer::stateChanged, this, [this, rendererPtr = renderer.get()]() {
        onRendererStateChanged(rendererPtr);
    });
}

void MediaRendererModel::onRendererDisconnected(std::shared_ptr<Multimedia::Renderer> const& renderer)
{
    auto const idx = indexOf(renderer.get());
    if (not idx.isValid()) {
        return;
    }

    disconnect(renderer.get(), nullptr, this, nullptr);
    beginRemoveRows(QModelIndex{}, idx.row(), idx.row());
    mRenderers.remove(idx.row());
    qCDebug(shell) << "Remove MediaRenderer index:" << idx.row() << "from renderers. Address:" << renderer.get();
    endRemoveRows();

    if (mActiveRenderer == renderer) {
        mActiveRenderer = nullptr;
        Q_EMIT activeRendererChanged();
    }
}

void MediaRendererModel::onDiscoveryFinished()
{
    if (not mScanning) {
        return;
    }

    mScanning = false;
    Q_EMIT scanningChanged();
}

void MediaRendererModel::onRendererStateChanged(Multimedia::Renderer const* renderer)
{
    auto const idx = indexOf(renderer);
    if (idx.isValid()) {
        Q_EMIT dataChanged(idx, idx, {static_cast<int>(DisplayRole::PlaybackState)});
    }
}

QModelIndex MediaRendererModel::indexOf(Multimedia::Renderer const* renderer) const noexcept
{
    if (renderer == nullptr) {
        return {};
    }

    auto const iter = std::find_if(mRenderers.cbegin(), mRenderers.cend(), [renderer](auto const& other) {
        return other.get() == renderer;
    });
    if (iter == mRenderers.cend()) {
        return {};
    }
    return index(static_cast<int>(std::distance(mRenderers.cbegin(), iter)));
}

} // namespace Shell
