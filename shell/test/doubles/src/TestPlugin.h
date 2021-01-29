// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "MultimediaPlugin.h"

namespace Shell
{

class TestPlugin : public PluginCore::MultimediaPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TestPlugin);

public:
    TestPlugin();
    ~TestPlugin() override;

    QString pluginName() const override;

    PluginCore::PluginVersion getPluginVersion() const override;

    QUuid getPluginIdentifier() const override;

    bool load(QQmlContext *context) override;

    bool unload() override;

    QUrl mainQMLUrl() const override;

    QUrl pluginIconUrl() const override;

    bool handleBackButton() override;

    bool handleBackButtonCalled{ false };
};

} // namespace Shell
#endif // TESTPLUGIN_H
