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
#include "MediaServerModel.h"
#include "IMediaServer.h"
#include <QUrl>

namespace MediaServer::Plugin
{

MediaServerModel::MediaServerModel()
    : QAbstractListModel()
{
}

MediaServerModel::~MediaServerModel() = default;

void MediaServerModel::insert(UPnPAV::IMediaServer *mediaServer) noexcept
{
    if((mediaServer == nullptr) || mIndexLookup.contains(mediaServer->name()))
    {
        return;
    }

    beginInsertRows(index(mMediaServer.size()), mMediaServer.size(), mMediaServer.size());
    mIndexLookup.insert(mediaServer->name(), mMediaServer.size());
    mMediaServer.insert(mMediaServer.size(), mediaServer);
    endInsertRows();
}

void MediaServerModel::removeServer(UPnPAV::IMediaServer *mediaServer) noexcept
{
    if(mediaServer == nullptr)
    {
        return;
    }

    auto mediaServerIndex = mIndexLookup.value(mediaServer->name());
    beginRemoveRows(index(mediaServerIndex), mediaServerIndex, mediaServerIndex);
    mMediaServer.remove(mediaServerIndex);
    endRemoveRows();
}

int MediaServerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mMediaServer.size();
}

QVariant MediaServerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant{};
    }

    if(role == MediaServerModel::MediaServerName)
    {
        return QVariant{ mMediaServer.value(index.row())->name() };
    }
    else if(role == MediaServerModel::MediaServerIconUrl)
    {
        return QVariant{ mMediaServer.value(index.row())->iconUrl() };
    }

    return QVariant{};
}

QHash<int, QByteArray> MediaServerModel::roleNames() const
{
    return { std::make_pair(MediaServerModel::MediaServerName, "mediaServerName"),
             std::make_pair(MediaServerModel::MediaServerIconUrl, "mediaServerIconUrl") };
}

} // namespace MediaServer::Plugin
