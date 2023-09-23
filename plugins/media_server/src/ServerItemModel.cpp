// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
    if (mMediaServerObjects.isEmpty() || !index.isValid())
    {
        return QVariant{};
    }

    const auto mediaServerObject = mMediaServerObjects.at(index.row());
    if (role == RoleName::ItemName)
    {
        return QVariant::fromValue<QString>(mediaServerObject.title());
    }
    else if (role == RoleName::ItemClass)
    {
        if (mediaServerObject.typeClass().contains("storageFolder"))
        {
            return QVariant::fromValue<ServerItemModel::ItemType>(ItemType::ItemFolder);
        }
        else
        {
            return QVariant::fromValue<ServerItemModel::ItemType>(ItemType::ItemFile);
        }
    }
    else if (role == RoleName::ItemId)
    {
        return QVariant::fromValue<QString>(mediaServerObject.id());
    }

    return QVariant{};
}

QHash<int, QByteArray> ServerItemModel::roleNames() const
{
    return QHash<qint32, QByteArray>{std::make_pair(ServerItemModel::ItemClass, "itemClass"),
                                     std::make_pair(ServerItemModel::ItemName, "itemName"),
                                     std::make_pair(ServerItemModel::ItemId, "itemId")};
}

void ServerItemModel::clearMediaServerObjects()
{
    beginResetModel();
    mMediaServerObjects.clear();
    endResetModel();
}

} // namespace MediaServer::Plugin
