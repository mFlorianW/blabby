#include "MediaServerModel.h"
#include "IMediaServer.h"
#include <QUrl>

MediaServerModel::MediaServerModel()
    : QAbstractListModel()
{
}

void MediaServerModel::insert(UPnPAV::IMediaServer *mediaServer)
{
    if(mediaServer == nullptr)
    {
        return;
    }

    mMediaServer.insert(mMediaServer.size(), mediaServer);
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
