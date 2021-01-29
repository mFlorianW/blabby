// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MULTIMEDIAPLUGINSOURCE_H
#define MULTIMEDIAPLUGINSOURCE_H

#include <MultimediaPlugin.h>
#include <QVector>
#include <memory>

namespace Shell
{

class MultiMediaPluginSource : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MultiMediaPluginSource)
public:
    MultiMediaPluginSource() = default;

    virtual ~MultiMediaPluginSource() = default;

    virtual void setPluginQQmlContext(QQmlContext *context) noexcept = 0;
    virtual void loadPlugins() const = 0;

    virtual QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> plugins() const = 0;
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINSOURCE_H
