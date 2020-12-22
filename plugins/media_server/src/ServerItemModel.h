/**
 ** This file is part of the Blabby project.
 ** Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
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
#ifndef SERVERITEMMODEL_H
#define SERVERITEMMODEL_H

#include "MediaServerObject.h"
#include <QAbstractListModel>

namespace MediaServerPlugin
{

class ServerItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ServerItemModel)
public:
    enum RoleName
    {
        ItemClass = Qt::UserRole + 1,
        ItemName
    };
    Q_ENUM(RoleName)

    enum ItemType
    {
        ItemFolder,
        ItemFile,
    };
    Q_ENUM(ItemType)

    ServerItemModel();

    void insertMediaServerObject(const UPnPAV::MediaServerObject &object);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<UPnPAV::MediaServerObject> mMediaServerObjects;
};

} // namespace MediaServerPlugin

#endif // SERVERITEMMODEL_H
