// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QmlSingletonRegistry.hpp"
#include "JsonRendererStore.hpp"
#include <QStandardPaths>

namespace Shell
{

namespace
{
std::shared_ptr<Multimedia::RendererStore> createRendererStore()
{
    auto const dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return std::make_shared<Multimedia::JsonRendererStore>(dataDir + QStringLiteral("/renderers.json"));
}
} // namespace

QmlSingletonRegistry::QmlSingletonRegistry()
    : mSourceModel{std::make_unique<Multimedia::ProviderLoader>()}
    , mRendererModel{std::make_unique<Multimedia::RendererProvider>(createRendererStore())}
    , mActiveRendererController{mRendererModel}
    , mQueueModel{mRendererModel}
{
    // The Library shows the Items of the Active Source.
    connect(&mSourceModel, &MediaSourceModel::activeMediaSourceChanged, &mItemModel, [this] {
        mItemModel.setMediaSource(mSourceModel.activeMediaSource());
    });
    // Tapping a Playable in the Library plays it through the Queue.
    connect(&mItemModel, &MediaItemModel::playRequested, &mQueueModel, &QueueModel::replace);
}

QmlSingletonRegistry::~QmlSingletonRegistry() = default;

MediaSourceModel* QmlSingletonRegistry::mediaSourceModel() noexcept
{
    return &mSourceModel;
}

MediaItemModel* QmlSingletonRegistry::mediaItemModel() noexcept
{
    return &mItemModel;
}

MediaRendererModel* QmlSingletonRegistry::mediaRendererModel() noexcept
{
    return &mRendererModel;
}

ActiveRendererController* QmlSingletonRegistry::activeRendererController() noexcept
{
    return &mActiveRendererController;
}

QueueModel* QmlSingletonRegistry::queueModel() noexcept
{
    return &mQueueModel;
}

QObject* QmlSingletonRegistry::createQmlRegistry(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QmlSingletonRegistry{}; // NOLINT cppcoreguidelines-owning-memory
}

} // namespace Shell
