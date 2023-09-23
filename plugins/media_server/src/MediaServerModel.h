// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MEDIASERVERMODEL_H
#define MEDIASERVERMODEL_H

#include <QAbstractListModel>

namespace UPnPAV
{
class IMediaServer;
}
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

    void insert(UPnPAV::IMediaServer *mediaServer) noexcept;
    void removeServer(UPnPAV::IMediaServer *mediaServer) noexcept;
    UPnPAV::IMediaServer *mediaServer(qint32 modelIndex) const noexcept;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<QString, qint32> mIndexLookup;
    QHash<qint32, UPnPAV::IMediaServer *> mMediaServer;
};

} // namespace MediaServer::Plugin

#endif // MEDIASERVERMODEL_H
