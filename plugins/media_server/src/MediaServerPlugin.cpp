// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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

namespace
{
constexpr char mediaServerView[] = "qrc:/mediaserver/qml/views/MediaServerView.qml";
constexpr char serverItemView[] = "qrc:/mediaserver/qml/views/ServerItemView.qml";
} // namespace

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

bool MediaServerPlugin::handleBackButton()
{
    if(activeView().toString() == serverItemView)
    {
        if(!mServerItemView->goPreviousFolder())
        {
            showMediaSeverView();
        }

        return true;
    }

    return false;
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

    setActiveView(QUrl{ mediaServerView });
}

void MediaServerPlugin::showServerItemView(qint32 mediaServerIndex)
{
    if(!mServerItemView && mQmlContext != nullptr)
    {
        mServerItemModel = std::make_unique<ServerItemModel>();
        mServerItemView = std::make_unique<ServerItemView>(mServerItemModel.get());

        mQmlContext->setContextProperty("g_ServerItemView", mServerItemView.get());
    }

    auto mediaServer = mMediaServerModel->mediaServer(mediaServerIndex);
    mServerItemView->setMediaServer(mediaServer);
    setActiveView(QUrl{ serverItemView });
}

} // namespace MediaServer::Plugin
