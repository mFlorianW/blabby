// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FILESYSTEMPLUGINSOURCE_H
#define FILESYSTEMPLUGINSOURCE_H

#include "MultiMediaPluginSource.h"
#include <QObject>
#include <memory>

namespace Shell
{
class FileSystemPluginSourcePrivate;

class FileSystemPluginSource : public MultiMediaPluginSource
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(FileSystemPluginSource)
public:
    FileSystemPluginSource();
    ~FileSystemPluginSource() override;

    void setPluginQQmlContext(QQmlContext *context) noexcept override;
    void loadPlugins() const override;

    QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> plugins() const override;

private:
    std::unique_ptr<FileSystemPluginSourcePrivate> d;
};

} // namespace Shell

#endif // FILESYSTEMPLUGINSOURCE_H
