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
#include "ServerItemModel.h"

namespace MediaServer::Plugin
{

ServerItemModel::ServerItemModel()
    : QAbstractListModel()
{
}

ServerItemModel::~ServerItemModel() = default;

void ServerItemModel::insertMediaServerObject(const UPnPAV::MediaServerObject &object)
{
    beginInsertRows(index(mMediaServerObjects.size()), mMediaServerObjects.size(), mMediaServerObjects.size());
    mMediaServerObjects.append(object);
    endInsertRows();
}

int ServerItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mMediaServerObjects.size();
}

QVariant ServerItemModel::data(const QModelIndex &index, int role) const
{
    if(mMediaServerObjects.isEmpty() || !index.isValid())
    {
        return QVariant{};
    }

    const auto mediaServerObject = mMediaServerObjects.at(index.row());
    if(role == RoleName::ItemName)
    {
        return mMediaServerObjects.at(index.row()).title();
    }
    else if(role == RoleName::ItemClass)
    {
        if(mediaServerObject.typeClass().contains("storageFolder"))
        {
            return QVariant::fromValue<ServerItemModel::ItemType>(ItemType::ItemFolder);
        }
        else
        {
            return QVariant::fromValue<ServerItemModel::ItemType>(ItemType::ItemFile);
        }
    }

    return QVariant{};
}

QHash<int, QByteArray> ServerItemModel::roleNames() const
{
    return QHash<qint32, QByteArray>{ std::make_pair(ServerItemModel::ItemClass, "itemClass"),
                                      std::make_pair(ServerItemModel::ItemName, "itemName") };
}

} // namespace MediaServer::Plugin
