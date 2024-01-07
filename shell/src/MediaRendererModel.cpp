// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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

    mProvider->discover();
}

MediaRendererModel::~MediaRendererModel() = default;

int MediaRendererModel::rowCount(QModelIndex const &parent) const noexcept
{
    Q_UNUSED(parent);
    return static_cast<int>(mRenderers.size());
}

QHash<int, QByteArray> MediaRendererModel::roleNames() const noexcept
{
    static auto const roles = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(DisplayRole::MediaRendererTitle), QByteArray{"mediaRendererTitle"}),
        std::make_pair(static_cast<int>(DisplayRole::MediaRendererIconUrl), QByteArray{"mediaRendererIconUrl"}),
        std::make_pair(static_cast<int>(DisplayRole::MediaRendererActive), QByteArray{"mediaRendererActive"}),
    };
    return roles;
}

QVariant MediaRendererModel::data(QModelIndex const &index, int role) const noexcept
{
    if (not index.isValid() or (index.row() >= mRenderers.size())) {
        qCritical(shell) << "Failed to request data. Error: invalid index:" << index.row()
                         << "Renderers size:" << mRenderers.size();
        return {};
    }

    auto const renderer = mRenderers.at(index.row());
    auto const dispRole = static_cast<DisplayRole>(role);
    if (dispRole == DisplayRole::MediaRendererTitle) {
        return renderer->name();
    } else if (dispRole == DisplayRole::MediaRendererIconUrl) {
        return renderer->iconUrl();
    } else if (dispRole == DisplayRole::MediaRendererActive) {
        return renderer == mActiveRenderer;
    }
    return {};
}

std::shared_ptr<Multimedia::Renderer> MediaRendererModel::activeRenderer() const noexcept
{
    return mActiveRenderer;
}

void MediaRendererModel::activateRenderer(QModelIndex const &index)
{
    if (index.isValid() and index.row() >= mRenderers.size()) {
        qCritical(shell) << "Failed to set active Renderer. Error: invalid index:" << index.row()
                         << "Renderers size:" << mRenderers.size();
        return;
    }

    auto const renderer = mRenderers.at(index.row());
    if (mActiveRenderer != renderer) {
        mActiveRenderer = renderer;
        qCDebug(shell) << "Activate renderer for index" << index.row() << ".";
        Q_EMIT activeRendererChanged();
        Q_EMIT dataChanged(index, index, {static_cast<int>(DisplayRole::MediaRendererActive)});
    }
}

void MediaRendererModel::onRendererConnected(std::shared_ptr<Renderer> const &renderer)
{
    auto const newIndex = static_cast<int>(mRenderers.size());
    beginInsertRows(index(newIndex), newIndex, newIndex);
    mRenderers.push_back(std::move(renderer));
    endInsertRows();
}

void MediaRendererModel::onRendererDisconnected(std::shared_ptr<Multimedia::Renderer> const &renderer)
{
    auto sourceIndex = std::find_if(mRenderers.cbegin(), mRenderers.cend(), [&](std::shared_ptr<Renderer> const &src) {
        return src == renderer;
    });
    if (sourceIndex != mRenderers.cend()) {
        auto const idx = static_cast<int>(std::distance(mRenderers.cbegin(), sourceIndex));
        beginRemoveRows(index(idx), idx, idx);
        mRenderers.remove(static_cast<int>(idx));
        qCDebug(shell) << "Remove MediaRenderer index:" << idx << "from renderers. Address:" << renderer.get()
                       << "iter:" << sourceIndex->get();
        endRemoveRows();
    }
}

} // namespace Shell
