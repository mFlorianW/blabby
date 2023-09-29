// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERPLUGIN_H
#define MEDIASERVERPLUGIN_H

#include "MultimediaPlugin.h"
#include <memory>
namespace UPnPAV
{
class ServiceProviderFactory;
class MediaServerFactory;
} // namespace UPnPAV

namespace MediaServer::Plugin
{
class MediaServerModel;
class MediaServerView;
class ServerItemView;
class ServerItemModel;

class MediaServerPlugin : public PluginCore::MultimediaPlugin
{
    Q_OBJECT
    Q_INTERFACES(PluginCore::MultimediaPlugin)
    Q_PLUGIN_METADATA(IID "de.blabby.MultimediaPlugin")
public:
    MediaServerPlugin();
    ~MediaServerPlugin() override;

    QString pluginName() const override;

    PluginCore::PluginVersion getPluginVersion() const override;

    QUuid getPluginIdentifier() const override;

    bool load(QQmlContext *context) override;

    bool unload() override;

    QUrl pluginIconUrl() const override;

    bool handleBackButton() override;

    Q_INVOKABLE void showMediaSeverView();
    Q_INVOKABLE void showServerItemView(qint32 mediaServerIndex);

private:
    std::unique_ptr<UPnPAV::ServiceProviderFactory> mServiceProviderFactory;
    std::unique_ptr<UPnPAV::MediaServerFactory> mMediaServerFactory;
    std::unique_ptr<MediaServerModel> mMediaServerModel;
    std::unique_ptr<MediaServerView> mMediaServerView;
    std::unique_ptr<ServerItemView> mServerItemView;
    std::unique_ptr<ServerItemModel> mServerItemModel;
    QQmlContext *mQmlContext{nullptr};
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERPLUGIN_H
