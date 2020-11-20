#include "MediaServerPlugin.h"
#include <QUrl>

MediaServerPlugin::MediaServerPlugin()
{
}

QString MediaServerPlugin::getPluginName() const
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

QUrl MediaServerPlugin::getMainQML() const
{
    return QUrl{ "qrc:/mediaserver/qml/MainWindow.qml" };
}
