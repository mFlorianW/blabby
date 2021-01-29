// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERVIEW_H
#define MEDIASERVERVIEW_H

#include <QQuickItem>
#include <map>
#include <memory>

namespace UPnPAV
{
class IServiceProviderFactory;
class IServiceProvider;
class IMediaServer;
class IMediaServerFactory;
} // namespace UPnPAV

namespace MediaServer::Plugin
{
class MediaServerModel;

class MediaServerView : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerView)
    Q_PROPERTY(MediaServer::Plugin::MediaServerModel *mediaServerModel READ mediaServerModel CONSTANT)
    Q_PROPERTY(UPnPAV::IMediaServer *activeMediaServer READ activeMediaServer NOTIFY activeMediaServerChanged)
public:
    MediaServerView(MediaServerModel *model, UPnPAV::IMediaServerFactory *mediaServerFab,
                    UPnPAV::IServiceProviderFactory *serviceProviderFab);
    ~MediaServerView() override;

    MediaServerModel *mediaServerModel() const noexcept;

    Q_INVOKABLE void searchMediaServer() const noexcept;

    Q_INVOKABLE void setActiveMediaServer(qint32 index) noexcept;
    UPnPAV::IMediaServer *activeMediaServer() const noexcept;

Q_SIGNALS:
    void activeMediaServerChanged();

private Q_SLOTS:
    void onServiceConnected(const QString &usn);
    void onServerDisconnected(const QString &usn);

private:
    UPnPAV::IServiceProviderFactory *mServiceProviderFactory;
    UPnPAV::IMediaServerFactory *mMediaServerFactory;
    MediaServerModel *mMediaServerModel;
    std::unique_ptr<UPnPAV::IServiceProvider> mServiceProvider;
    std::map<QString, std::unique_ptr<UPnPAV::IMediaServer>> mMediaServers;
    qint32 mActiveIndex{ -1 };
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERVIEW_H
