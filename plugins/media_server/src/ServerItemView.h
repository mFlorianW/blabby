// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
