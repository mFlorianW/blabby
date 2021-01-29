// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEMORYPLUGINSOURCE_H
#define MEMORYPLUGINSOURCE_H

#include "MultiMediaPluginSource.h"
#include "TestPlugin.h"

namespace Shell
{

class MemoryPluginSource : public MultiMediaPluginSource
{
public:
    MemoryPluginSource();

    void setPluginQQmlContext(QQmlContext *context) noexcept override;
    void loadPlugins() const override;
    QVector<std::shared_ptr<PluginCore::MultimediaPlugin> > plugins() const override;

    std::shared_ptr<TestPlugin> plugin;
};

} //namespace Shell
#endif // MEMORYPLUGINSOURCE_H
