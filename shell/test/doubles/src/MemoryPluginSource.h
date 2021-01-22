/**
 ** This file is part of the Blabby project.
 ** Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
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
