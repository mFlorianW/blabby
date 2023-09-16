// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
    if ((mediaServer == nullptr) || mIndexLookup.contains(mediaServer->name()))
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
    if (mediaServer == nullptr)
    {
        return;
    }

    auto mediaServerIndex = mIndexLookup.value(mediaServer->name());
    beginRemoveRows(index(mediaServerIndex), mediaServerIndex, mediaServerIndex);
    mMediaServer.remove(mediaServerIndex);
    endRemoveRows();
}

UPnPAV::IMediaServer *MediaServerModel::mediaServer(qint32 modelIndex) const noexcept
{
    auto itemIndex = index(modelIndex);
    if (itemIndex.isValid())
    {
        return mMediaServer.value(itemIndex.row());
    }

    return nullptr;
}

int MediaServerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mMediaServer.size();
}

QVariant MediaServerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant{};
    }

    if (role == MediaServerModel::MediaServerName)
    {
        return QVariant{mMediaServer.value(index.row())->name()};
    }
    else if (role == MediaServerModel::MediaServerIconUrl)
    {
        return QVariant{mMediaServer.value(index.row())->iconUrl()};
    }

    return QVariant{};
}

QHash<int, QByteArray> MediaServerModel::roleNames() const
{
    return {std::make_pair(MediaServerModel::MediaServerName, "mediaServerName"),
            std::make_pair(MediaServerModel::MediaServerIconUrl, "mediaServerIconUrl")};
}

} // namespace MediaServer::Plugin
