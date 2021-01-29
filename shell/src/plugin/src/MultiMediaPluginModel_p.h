// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MULTIMEDIAPLUGINMODELPRIVATE_H
#define MULTIMEDIAPLUGINMODELPRIVATE_H

#include <QVector>
#include <memory>

namespace PluginCore
{
class MultimediaPlugin;
}

namespace Shell
{

class MultiMediaPluginModelPrivate
{
public:
    MultiMediaPluginModelPrivate(const QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> &plugins);

    QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> mPlugins;
    qint32 mActiveIndex{ -1 };
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINMODELPRIVATE_H
