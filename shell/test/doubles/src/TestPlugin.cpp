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
#include "TestPlugin.h"
#include <QUrl>

namespace Shell
{

TestPlugin::TestPlugin()
    : PluginCore::MultimediaPlugin()
{
}

TestPlugin::~TestPlugin() = default;

QString TestPlugin::pluginName() const
{
    return "TestPlugin";
}

PluginCore::PluginVersion TestPlugin::getPluginVersion() const
{
    return { .major = 1, .minor = 0, .patch = 0 };
}

QUuid TestPlugin::getPluginIdentifier() const
{
    return { "d6d02a93-b6f0-4581-ba0a-b70397c1200a" };
}

bool TestPlugin::load(QQmlContext *context)
{
    Q_UNUSED(context);
    return true;
}

bool Shell::TestPlugin::unload()
{
    return true;
}

QUrl TestPlugin::mainQMLUrl() const
{
    return QUrl{ "qrc:/qml/main.qml" };
}

QUrl TestPlugin::pluginIconUrl() const
{
    return QUrl{ "qrc:/icon/pluginIcon.png" };
}

} // namespace Shell
