// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MemoryPluginSource.h"
#include "TestPlugin.h"

namespace Shell
{

MemoryPluginSource::MemoryPluginSource()
{
    plugin = std::make_shared<TestPlugin>();
}

void MemoryPluginSource::setPluginQQmlContext(QQmlContext *context) noexcept
{
    Q_UNUSED(context);
}

void MemoryPluginSource::loadPlugins() const
{
}

QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> MemoryPluginSource::plugins() const
{
    return {plugin};
}

} // namespace Shell
