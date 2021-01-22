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
