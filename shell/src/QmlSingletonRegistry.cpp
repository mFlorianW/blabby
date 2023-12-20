// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "QmlSingletonRegistry.hpp"

namespace Shell
{

QmlSingletonRegistry::QmlSingletonRegistry()
    : mSourceModel{std::make_unique<Multimedia::ProviderLoader>()}
    , mRendererModel{std::make_unique<Multimedia::RendererProvider>()}
{
}

QmlSingletonRegistry::~QmlSingletonRegistry() = default;

MediaSourceModel *QmlSingletonRegistry::mediaSourceModel() noexcept
{
    return &mSourceModel;
}

MediaItemModel *QmlSingletonRegistry::mediaItemModel() noexcept
{
    return &mItemModel;
}

MediaRendererModel *QmlSingletonRegistry::mediaRendererModel() noexcept
{
    return &mRendererModel;
}

QObject *QmlSingletonRegistry::createQmlRegistry(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QmlSingletonRegistry{};
}

} // namespace Shell
