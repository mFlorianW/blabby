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
#ifndef MEDIASERVERMODEL_H
#define MEDIASERVERMODEL_H

#include <QAbstractListModel>

namespace UPnPAV
{
class IMediaServer;
}

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

    void insert(UPnPAV::IMediaServer *mediaServer);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<qint32, UPnPAV::IMediaServer *> mMediaServer;
};

#endif // MEDIASERVERMODEL_H
