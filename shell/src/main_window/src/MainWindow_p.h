// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MAINWINDOWPRIVATE_H
#define MAINWINDOWPRIVATE_H
#include "MultiMediaPluginModel.h"
#include "MultiMediaPluginSource.h"
#include "MultimediaPlugin.h"

namespace Shell
{

class MainWindowPrivate
{
    Q_DISABLE_COPY_MOVE(MainWindowPrivate)
public:
    MainWindowPrivate(Shell::MultiMediaPluginModel *model, Shell::MultiMediaPluginSource *pluginSource);
    ~MainWindowPrivate();

    void loadPlugins();

    MultiMediaPluginSource *mSource{ nullptr };
    MultiMediaPluginModel *mModel{ nullptr };
    QUrl mActivePluginUrl{ "" };
    QString mActivePluginName{ "" };
    PluginCore::MultimediaPlugin *mActivePlugin{ nullptr };
};

} // namespace Shell

#endif // MAINWINDOWPRIVATE_H
