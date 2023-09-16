// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "TestPlugin.h"
#include <QUrl>

namespace Shell
{

TestPlugin::TestPlugin()
    : PluginCore::MultimediaPlugin()
{
}

TestPlugin::~TestPlugin() = default;

QString TestPlugin::pluginName() const
{
    return "TestPlugin";
}

PluginCore::PluginVersion TestPlugin::getPluginVersion() const
{
    return {.major = 1, .minor = 0, .patch = 0};
}

QUuid TestPlugin::getPluginIdentifier() const
{
    return {"d6d02a93-b6f0-4581-ba0a-b70397c1200a"};
}

bool TestPlugin::load(QQmlContext *context)
{
    Q_UNUSED(context);
    return true;
}

bool Shell::TestPlugin::unload()
{
    return true;
}

QUrl TestPlugin::pluginIconUrl() const
{
    return QUrl{"qrc:/icon/pluginIcon.png"};
}

bool TestPlugin::handleBackButton()
{
    handleBackButtonCalled = true;
    return true;
}

} // namespace Shell
