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
#ifndef MAINCONTROLLER__H
#define MAINCONTROLLER__H

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

namespace MediaServerPlugin
{
class MediaServerModel;

class MainController : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainController)
    Q_PROPERTY(UPnPAV::IServiceProviderFactory *serviceProviderFactory READ serviceProviderFactory WRITE setServiceProviderFactory NOTIFY serviceProviderChanged)
    Q_PROPERTY(UPnPAV::IMediaServerFactory *mediaServerFactory READ mediaServerFactory WRITE setMediaServerFactory NOTIFY mediaServerFactoryChanged)
    Q_PROPERTY(MediaServerPlugin::MediaServerModel *mediaServerModel READ mediaServerModel WRITE setMediaServerModel NOTIFY mediaServerModelChanged)
public:
    MainController();
    ~MainController() override;

    UPnPAV::IServiceProviderFactory *serviceProviderFactory() const noexcept;
    void setServiceProviderFactory(UPnPAV::IServiceProviderFactory *serviceProviderFactory) noexcept;

    UPnPAV::IMediaServerFactory *mediaServerFactory() const noexcept;
    void setMediaServerFactory(UPnPAV::IMediaServerFactory *mediaServerFactory) noexcept;

    MediaServerModel *mediaServerModel() const noexcept;
    void setMediaServerModel(MediaServerModel *mediaServerModel) noexcept;

    Q_INVOKABLE void searchMediaServer() const noexcept;

Q_SIGNALS:
    void serviceProviderChanged();
    void mediaServerFactoryChanged();
    void mediaServerModelChanged();

private Q_SLOTS:
    void onServiceConnected(const QString &usn);

private:
    UPnPAV::IServiceProviderFactory *mServiceProviderFactory;
    UPnPAV::IMediaServerFactory *mMediaServerFactory;
    MediaServerModel *mMediaServerModel;
    std::unique_ptr<UPnPAV::IServiceProvider> mServiceProvider;
    std::map<QString, std::unique_ptr<UPnPAV::IMediaServer>> mMediaServers;
};

} // namespace MediaServerPlugin

#endif // MAINCONTROLLER__H
