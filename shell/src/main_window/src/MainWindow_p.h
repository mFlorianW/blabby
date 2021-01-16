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
