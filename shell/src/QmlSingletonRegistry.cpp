// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QmlSingletonRegistry.hpp"
#include "BlabbyVersion.hpp"
#include "IniSettingsStore.hpp"
#include "JsonRendererStore.hpp"
#include "LoadProviders.hpp"
#include "ProviderDirs.hpp"
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

std::shared_ptr<SettingsStore> createSettingsStore()
{
    auto const dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return std::make_shared<IniSettingsStore>(dataDir + QStringLiteral("/settings.ini"));
}

Multimedia::Providers loadInstalledProviders()
{
    auto loader = Multimedia::ProviderLoader{};
    return loadProviders(loader, {QStringLiteral(DEFAULT_PROVIDER_DIR)});
}
} // namespace

QmlSingletonRegistry::QmlSingletonRegistry()
    : mProviders{loadInstalledProviders()}
    , mSettingsStore{createSettingsStore()}
    , mSourceModel{mProviders}
    , mRendererModel{std::make_unique<Multimedia::RendererProvider>(createRendererStore()), mSettingsStore}
    , mActiveRendererController{mRendererModel}
    , mQueueModel{mRendererModel}
    , mSettingsModel{mProviders, QString::fromLatin1(blabbyVersion), mSettingsStore}
{
    // The Library shows the Items of the Active Source.
    connect(&mSourceModel, &MediaSourceModel::activeMediaSourceChanged, &mItemModel, [this] {
        mItemModel.setMediaSource(mSourceModel.activeMediaSource());
    });
    // Tapping a Playable in the Library appends it to the Queue and plays it.
    connect(&mItemModel, &MediaItemModel::playRequested, &mQueueModel, &QueueModel::appendAndPlay);
    // The menu of a Playable or a Container in the Library plays it next or adds it to the end of the Queue.
    connect(&mItemModel, &MediaItemModel::playNextRequested, &mQueueModel, &QueueModel::playNext);
    connect(&mItemModel, &MediaItemModel::addToQueueRequested, &mQueueModel, &QueueModel::addToQueue);
}

QmlSingletonRegistry::~QmlSingletonRegistry() = default;

Multimedia::Providers const& QmlSingletonRegistry::providers() const noexcept
{
    return mProviders;
}

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

SettingsModel* QmlSingletonRegistry::settingsModel() noexcept
{
    return &mSettingsModel;
}

QObject* QmlSingletonRegistry::createQmlRegistry(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QmlSingletonRegistry{}; // NOLINT cppcoreguidelines-owning-memory
}

} // namespace Shell
