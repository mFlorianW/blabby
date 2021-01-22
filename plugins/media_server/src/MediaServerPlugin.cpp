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
#include "MediaServer.h"
#include "MediaServerModel.h"
#include "MediaServerView.h"
#include "ServerItemModel.h"
#include "ServerItemView.h"
#include "ServiceProvider.h"
#include <QQmlContext>
#include <QUrl>
#include <qqml.h>

namespace MediaServer::Plugin
{

MediaServerPlugin::MediaServerPlugin()
{
}

MediaServerPlugin::~MediaServerPlugin() = default;

QString MediaServerPlugin::pluginName() const
{
    return QStringLiteral("Media Server");
}

PluginCore::PluginVersion MediaServerPlugin::getPluginVersion() const
{
    return PluginCore::PluginVersion{ .major = 0, .minor = 1, .patch = 0 };
}

QUuid MediaServerPlugin::getPluginIdentifier() const
{
    return QUuid{ "b53b2ffe-4f00-4d4d-b2a4-7b769a0744ea" };
}

bool MediaServerPlugin::load(QQmlContext *context)
{
    mQmlContext = context;
    const char pluginUrl[] = "de.blabby.mediaserverplugin";

    qmlRegisterUncreatableType<ServerItemModel>(pluginUrl, 1, 0, "ServerItemModel", "");
    qmlRegisterUncreatableType<MediaServerModel>(pluginUrl, 1, 0, "MediaServerModel", "");
    qmlRegisterUncreatableType<UPnPAV::IMediaServer>(pluginUrl, 1, 0, "IMediaServer", "");

    mQmlContext->setContextProperty("g_MediaServerPlugin", this);
    showMediaSeverView();

    return true;
}

bool MediaServerPlugin::unload()
{
    return true;
}

QUrl MediaServerPlugin::mainQMLUrl() const
{
    return QUrl{};
}

QUrl MediaServerPlugin::pluginIconUrl() const
{
    return QUrl{ "qrc:/mediaserver/icon/MediaServerPlugin.png" };
}

void MediaServerPlugin::showMediaSeverView()
{
    if(!mMediaServerView && mQmlContext != nullptr)
    {
        mServiceProviderFactory = std::make_unique<UPnPAV::ServiceProviderFactory>();
        mMediaServerFactory = std::make_unique<UPnPAV::MediaServerFactory>();
        mMediaServerModel = std::make_unique<MediaServerModel>();
        mMediaServerView = std::make_unique<MediaServerView>(mMediaServerModel.get(), mMediaServerFactory.get(),
                                                             mServiceProviderFactory.get());

        mQmlContext->setContextProperty("g_MediaServerView", mMediaServerView.get());
    }

    setActiveView(QUrl{ "qrc:/mediaserver/qml/views/MediaServerView.qml" });
}

void MediaServerPlugin::showServerItemView(UPnPAV::IMediaServer *mediaServer)
{
    if(!mServerItemView && mQmlContext != nullptr)
    {
        mServerItemModel = std::make_unique<ServerItemModel>();
        mServerItemView = std::make_unique<ServerItemView>(mServerItemModel.get());

        mQmlContext->setContextProperty("g_ServerItemView", mServerItemView.get());
    }

    mServerItemView->setMediaServer(mediaServer);
    setActiveView(QUrl{ "qrc:/mediaserver/qml/views/ServerItemView.qml" });
}

} // namespace MediaServer::Plugin
