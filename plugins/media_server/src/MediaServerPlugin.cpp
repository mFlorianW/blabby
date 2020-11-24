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
#include "MediaServerPlugin.h"
#include <QUrl>

namespace MediaServerPlugin
{

MediaServerPlugin::MediaServerPlugin()
{
}

QString MediaServerPlugin::pluginName() const
{
    return QStringLiteral("MediaServer");
}

PluginCore::PluginVersion MediaServerPlugin::getPluginVersion() const
{
    return PluginCore::PluginVersion{ .major = 0, .minor = 1, .patch = 0 };
}

QUuid MediaServerPlugin::getPluginIdentifier() const
{
    return QUuid{ "b53b2ffe-4f00-4d4d-b2a4-7b769a0744ea" };
}

bool MediaServerPlugin::load()
{
    return true;
}

bool MediaServerPlugin::unload()
{
    return true;
}

QUrl MediaServerPlugin::mainQMLUrl() const
{
    return QUrl{ "qrc:/mediaserver/qml/MainWindow.qml" };
}

QUrl MediaServerPlugin::pluginIconUrl() const
{
    return QUrl{ "qrc:/mediaserver/icon/MediaServerPlugin.png" };
}

} // namespace MediaServerPlugin
