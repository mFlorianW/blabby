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
#ifndef SERVERITEMVIEW_H
#define SERVERITEMVIEW_H

#include <QQuickItem>
#include <QSharedPointer>
#include <QStack>

namespace UPnPAV
{
class IMediaServer;
class PendingSoapCall;
} // namespace UPnPAV

namespace MediaServer::Plugin
{
class ServerItemModel;

class ServerItemView : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UPnPAV::IMediaServer *mediaServer READ mediaServer WRITE setMediaServer NOTIFY mediaServerChanged)
    Q_PROPERTY(MediaServer::Plugin::ServerItemModel *itemModel READ serverItemModel CONSTANT)
public:
    ServerItemView(ServerItemModel *model);
    ~ServerItemView();

    UPnPAV::IMediaServer *mediaServer() const noexcept;
    void setMediaServer(UPnPAV::IMediaServer *mediaServer) noexcept;

    MediaServer::Plugin::ServerItemModel *serverItemModel() const noexcept;

    Q_INVOKABLE void requestStorageFolder(const QString &id);

    Q_INVOKABLE bool goPreviousFolder();

Q_SIGNALS:
    void mediaServerChanged();

private Q_SLOTS:
    void onBrowsRequestFinished();

private:
    void requestFolder(const QString &id);

private:
    UPnPAV::IMediaServer *mMediaServer{ nullptr };
    QSharedPointer<UPnPAV::PendingSoapCall> mPendingSoapCall{ nullptr };
    ServerItemModel *mServerItemModel{ nullptr };
    QString mActiveId;
    QStack<QString> mIdStack;
};

} // namespace MediaServer::Plugin

#endif // SERVERITEMCONTROLLER_H
