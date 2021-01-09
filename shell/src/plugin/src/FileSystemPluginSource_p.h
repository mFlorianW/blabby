/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
