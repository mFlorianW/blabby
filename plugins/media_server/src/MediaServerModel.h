// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERMODEL_H
#define MEDIASERVERMODEL_H

#include <MediaServer.h>
#include <QAbstractListModel>

namespace MediaServer::Plugin
{

class MediaServerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaServerModel)
public:
    enum RoleName
    {
        MediaServerName = Qt::UserRole + 1,
        MediaServerIconUrl,
    };
    Q_ENUM(RoleName)

    MediaServerModel();
    ~MediaServerModel() override;

    void insert(UPnPAV::MediaServer *mediaServer) noexcept;
    void removeServer(UPnPAV::MediaServer *mediaServer) noexcept;
    UPnPAV::MediaServer *mediaServer(qint32 modelIndex) const noexcept;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<QString, qint32> mIndexLookup;
    QHash<qint32, UPnPAV::MediaServer *> mMediaServer;
};

} // namespace MediaServer::Plugin

Q_DECLARE_OPAQUE_POINTER(MediaServer::Plugin::MediaServerModel);

#endif // MEDIASERVERMODEL_H
