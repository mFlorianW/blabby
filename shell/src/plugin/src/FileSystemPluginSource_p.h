// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FILESYSTEMPLUGINSOURCE_P_H
#define FILESYSTEMPLUGINSOURCE_P_H

#include "MultimediaPlugin.h"
#include <QVector>
#include <memory>

class QFileInfo;

namespace Shell
{
class FileSystemPluginSourcePrivate
{
public:
    FileSystemPluginSourcePrivate();
    ~FileSystemPluginSourcePrivate();

    void loadPlugins(const QFileInfo &fileInfo);

    QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> mLoadedPlugins;
    QQmlContext *mContext;
};
} // namespace Shell
#endif // FILESYSTEMPLUGINSOURCE_P_H
