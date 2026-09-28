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

namespace
{
/**
 * Online Renderers come before Offline ones, each ordered alphabetically by name.
 */
bool isOrderedBefore(Renderer const& lhs, Renderer const& rhs)
{
    if (lhs.availability() != rhs.availability()) {
        return lhs.availability() == Renderer::Availability::Online;
    }
    return lhs.name().compare(rhs.name(), Qt::CaseInsensitive) < 0;
}
} // namespace

MediaRendererModel::MediaRendererModel(std::unique_ptr<RendererProvider> provider)
    : mProvider{std::move(provider)}
{
    Q_ASSERT(mProvider != nullptr);

    for (auto const& renderer : mProvider->renderers()) {
        addRenderer(renderer);
    }

    connect(mProvider.get(), &RendererProvider::rendererConnected, this, &MediaRendererModel::onRendererConnected);
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
        std::make_pair(static_cast<int>(DisplayRole::Availability), QByteArray{"availability"}),
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
    } else if (dispRole == DisplayRole::Availability) {
        return static_cast<int>(renderer->availability());
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

    if (renderer->availability() == Renderer::Availability::Offline) {
        qCWarning(shell) << "Failed to set active Renderer. Error: Renderer" << renderer->name() << "is Offline.";
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
    // A known Renderer that is discovered again is already listed and was moved on its Availability change.
    if (not indexOf(renderer.get()).isValid()) {
        addRenderer(renderer);
    }
}

void MediaRendererModel::addRenderer(std::shared_ptr<Renderer> const& renderer)
{
    auto const newRow = sortedRow(renderer.get());
    beginInsertRows(QModelIndex{}, newRow, newRow);
    mRenderers.insert(newRow, renderer);
    endInsertRows();

    auto* const rendererPtr = renderer.get();
    connect(rendererPtr, &Renderer::stateChanged, this, [this, rendererPtr]() {
        onRendererStateChanged(rendererPtr);
    });
    connect(rendererPtr, &Renderer::availabilityChanged, this, [this, rendererPtr]() {
        onRendererChanged(rendererPtr);
    });
    connect(rendererPtr, &Renderer::detailsChanged, this, [this, rendererPtr]() {
        onRendererChanged(rendererPtr);
    });
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

void MediaRendererModel::onRendererChanged(Multimedia::Renderer const* renderer)
{
    auto const oldRow = indexOf(renderer).row();
    if (oldRow < 0) {
        return;
    }

    auto const newRow = sortedRow(renderer);
    if (newRow != oldRow) {
        // For moves downwards the destination is the row before which the moved row is placed in the old order.
        beginMoveRows(QModelIndex{}, oldRow, oldRow, QModelIndex{}, newRow > oldRow ? newRow + 1 : newRow);
        mRenderers.move(oldRow, newRow);
        endMoveRows();
    }

    auto const idx = index(newRow);
    Q_EMIT dataChanged(idx,
                       idx,
                       {static_cast<int>(DisplayRole::Name),
                        static_cast<int>(DisplayRole::Manufacturer),
                        static_cast<int>(DisplayRole::ModelName),
                        static_cast<int>(DisplayRole::Address),
                        static_cast<int>(DisplayRole::Availability)});

    if (mActiveRenderer.get() == renderer and renderer->availability() == Renderer::Availability::Offline) {
        clearActiveRenderer();
    }
}

int MediaRendererModel::sortedRow(Multimedia::Renderer const* renderer) const noexcept
{
    // The Renderer goes behind all other Renderers that are ordered before or equally to it,
    // so equally ordered Renderers stay in the order they appeared.
    auto const row = std::count_if(mRenderers.cbegin(), mRenderers.cend(), [renderer](auto const& other) {
        return other.get() != renderer and not isOrderedBefore(*renderer, *other);
    });
    return static_cast<int>(row);
}

void MediaRendererModel::clearActiveRenderer()
{
    auto const previousIndex = indexOf(mActiveRenderer.get());
    mActiveRenderer = nullptr;
    qCDebug(shell) << "Clear the Active Renderer.";
    Q_EMIT activeRendererChanged();
    if (previousIndex.isValid()) {
        Q_EMIT dataChanged(previousIndex, previousIndex, {static_cast<int>(DisplayRole::Active)});
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
