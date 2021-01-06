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
#ifndef MEDIASERVERPLUGIN_H
#define MEDIASERVERPLUGIN_H

#include "MultiMediaPlugin.h"
#include <memory>
namespace UPnPAV
{
class ServiceProviderFactory;
class MediaServerFactory;
} // namespace UPnPAV

namespace MediaServer::Plugin
{
class MediaServerModel;
class MainWindow;

class MediaServerPlugin : public QObject, PluginCore::MultiMediaPlugin
{
    Q_OBJECT
    Q_INTERFACES(PluginCore::MultiMediaPlugin)
    Q_PLUGIN_METADATA(IID "de.blabby.MultiMediaPlugin")
public:
    MediaServerPlugin();
    ~MediaServerPlugin() override;

    QString pluginName() const override;

    PluginCore::PluginVersion getPluginVersion() const override;

    QUuid getPluginIdentifier() const override;

    bool load(QQmlContext *context) override;

    bool unload() override;

    QUrl mainQMLUrl() const override;

    QUrl pluginIconUrl() const override;

private:
    std::unique_ptr<UPnPAV::ServiceProviderFactory> mServiceProviderFactory;
    std::unique_ptr<UPnPAV::MediaServerFactory> mMediaServerFactory;
    std::unique_ptr<MediaServerModel> mMediaServerModel;
    std::unique_ptr<MainWindow> mMainController;
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERPLUGIN_H
