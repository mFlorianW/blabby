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
#include "MemoryPluginSource.h"
#include "TestPlugin.h"

namespace Shell
{

MemoryPluginSource::MemoryPluginSource()
{
    plugin = std::make_shared<TestPlugin>();
}

void MemoryPluginSource::setPluginQQmlContext(QQmlContext *context) noexcept
{
    Q_UNUSED(context);
}

void MemoryPluginSource::loadPlugins() const
{
}

QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> MemoryPluginSource::plugins() const
{
    return { plugin };
}

} // namespace Shell
